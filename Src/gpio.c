
#include "includes.h"
/* Table of base addresses for GPIO instances. */
GPIO_Type * const g_gpioBase[GPIO_INSTANCE_COUNT] = GPIO_BASE_PTRS;

/* Table of base addresses for PORT instances. */
PORT_Type * const g_portBase[PORT_INSTANCE_COUNT] = PORT_BASE_PTRS;

/* Table to save port IRQ enum numbers defined in CMSIS files. */
const IRQn_Type g_portIrqId[PORT_INSTANCE_COUNT] = PORT_IRQS;


/*FUNCTION**********************************************************************
 *
 * Function Name : GPIO_Init
 * Description   : Initialize all GPIO pins used by board.
 * To initialize the GPIO driver, two arrays similar with
 * gpio_input_pin_user_config_t inputPin[] and
 * gpio_output_pin_user_config_t outputPin[] should be defined in user's file.
 * Then simply call GPIO_Init() and pass into these two arrays. If input
 * or output pins is not needed, pass in a NULL.
 *
 *END**************************************************************************/
void GPIO_Init(const gpio_input_pin_user_config_t * inputPins, const gpio_output_pin_user_config_t * outputPins)
{
    if (inputPins)
    {
        /* Initialize input pins.*/
        while (inputPins->pinName != GPIO_PINS_OUT_OF_RANGE)
        {
            GPIO_InputPinInit(inputPins++);
        }
    }

    if (outputPins)
    {
        /* Initialize output pins.*/
        while (outputPins->pinName != GPIO_PINS_OUT_OF_RANGE)
        {
            GPIO_OutputPinInit(outputPins++);
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : GPIO_InputPinInit
 * Description   : Initialize one GPIO input pin used by board.
 *
 *END**************************************************************************/
void GPIO_InputPinInit(const gpio_input_pin_user_config_t *inputPin)
{
    /* Get actual port and pin number.*/
    uint32_t port = GPIO_EXTRACT_PORT(inputPin->pinName);
    uint32_t pin = GPIO_EXTRACT_PIN(inputPin->pinName);
    GPIO_Type * gpioBase = g_gpioBase[port];
    PORT_Type * portBase = g_portBase[port];

    /* Un-gate port clock*/
    //CLOCK_SYS_EnablePortClock(port);

    /* Set current pin as gpio.*/
    PORT_Hal_SetMuxMode(portBase, pin, portMuxAsGpio);

    /* Set current pin as digital input.*/
    GPIO_Hal_SetPinDir(gpioBase, pin, gpioDigitalInput);

    /* Configure GPIO input features. */

    PORT_Hal_SetPullCmd(portBase, pin, inputPin->config.isPullEnable);

    PORT_Hal_SetPullMode(portBase, pin, inputPin->config.pullSelect);

    PORT_Hal_SetPassiveFilterCmd(portBase, pin, inputPin->config.isPassiveFilterEnabled);

    PORT_Hal_SetPinIntMode(portBase, pin, inputPin->config.interrupt);

    /* Configure NVIC */
    if ((inputPin->config.interrupt) && (g_portIrqId[port]))
    {
        /* Enable GPIO interrupt.*/
        NVIC_EnableIRQ(g_portIrqId[port]); //?????
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : GPIO_OutputPinInit
 * Description   : Initialize one GPIO output pin used by board.
 *
 *END**************************************************************************/
void GPIO_OutputPinInit(const gpio_output_pin_user_config_t *outputPin)
{
    /* Get actual port and pin number.*/
    uint32_t port = GPIO_EXTRACT_PORT(outputPin->pinName);
    uint32_t pin = GPIO_EXTRACT_PIN(outputPin->pinName);
    GPIO_Type * gpioBase = g_gpioBase[port];
    PORT_Type * portBase = g_portBase[port];

    /* Un-gate port clock*/
    //CLOCK_SYS_EnablePortClock(port);	//????

    /* Set current pin as gpio.*/
    PORT_Hal_SetMuxMode(portBase, pin, portMuxAsGpio);

    /* Set current pin as digital output.*/
    GPIO_Hal_SetPinDir(gpioBase, pin, gpioDigitalOutput);

    /* Configure GPIO output features. */
    GPIO_Hal_WritePinOutput(gpioBase, pin, outputPin->config.outputLogic);

    PORT_Hal_SetSlewRateMode(portBase, pin, outputPin->config.slewRate);

    PORT_Hal_SetDriveStrengthMode(portBase, pin, outputPin->config.driveStrength);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : GPIO_GetPinDir
 * Description   : Get current direction of individual GPIO pin.
 *
 *END**************************************************************************/
gpio_pin_direction_t GPIO_GetPinDir(uint32_t pinName)
{
    GPIO_Type * gpioBase = g_gpioBase[GPIO_EXTRACT_PORT(pinName)];
    uint32_t pin = GPIO_EXTRACT_PIN(pinName);

    return GPIO_Hal_GetPinDir(gpioBase, pin);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : GPIO_SetPinDir
 * Description   : Set current direction of individual GPIO pin.
 *
 *END**************************************************************************/
void GPIO_SetPinDir(uint32_t pinName, gpio_pin_direction_t direction)
{
    GPIO_Type * gpioBase = g_gpioBase[GPIO_EXTRACT_PORT(pinName)];
    uint32_t pin = GPIO_EXTRACT_PIN(pinName);

    GPIO_Hal_SetPinDir(gpioBase, pin, direction);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : GPIO_WritePinOutput
 * Description   : Set output level of individual GPIO pin to logic 1 or 0.
 *
 *END**************************************************************************/
void GPIO_WritePinOutput(uint32_t pinName, uint32_t output)
{
    GPIO_Type * gpioBase = g_gpioBase[GPIO_EXTRACT_PORT(pinName)];
    uint32_t pin = GPIO_EXTRACT_PIN(pinName);

    GPIO_Hal_WritePinOutput(gpioBase, pin, output);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : GPIO_SetPinOutput
 * Description   : Set output level of individual GPIO pin to logic 1.
 *
 *END**************************************************************************/
void GPIO_SetPinOutput(uint32_t pinName)
{
    GPIO_Type * gpioBase = g_gpioBase[GPIO_EXTRACT_PORT(pinName)];
    uint32_t pin = GPIO_EXTRACT_PIN(pinName);

    GPIO_Hal_SetPinOutput(gpioBase, pin);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : GPIO_ClearPinOutput
 * Description   : Set output level of individual GPIO pin to logic 0.
 *
 *END**************************************************************************/
void GPIO_ClearPinOutput(uint32_t pinName)
{
    GPIO_Type * gpioBase = g_gpioBase[GPIO_EXTRACT_PORT(pinName)];
    uint32_t pin = GPIO_EXTRACT_PIN(pinName);

    GPIO_Hal_ClearPinOutput(gpioBase, pin);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : GPIO_TogglePinOutput
 * Description   : Reverse current output logic of individual GPIO pin.
 *
 *END**************************************************************************/
void GPIO_TogglePinOutput(uint32_t pinName)
{
    GPIO_Type * gpioBase = g_gpioBase[GPIO_EXTRACT_PORT(pinName)];
    uint32_t pin = GPIO_EXTRACT_PIN(pinName);

    GPIO_Hal_TogglePinOutput(gpioBase, pin);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : GPIO_ReadPinInput
 * Description   : Read current input value of individual GPIO pin.
 *
 *END**************************************************************************/
uint32_t GPIO_ReadPinInput(uint32_t pinName)
{
    GPIO_Type * gpioBase = g_gpioBase[GPIO_EXTRACT_PORT(pinName)];
    uint32_t pin = GPIO_EXTRACT_PIN(pinName);

    return GPIO_Hal_ReadPinInput(gpioBase, pin);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : GPIO_IsPinIntPending
 * Description   : Read the individual pin-interrupt status flag.
 *
 *END**************************************************************************/
uint32_t GPIO_IsPinIntPending(uint32_t pinName)
{
    PORT_Type * portBase = g_portBase[GPIO_EXTRACT_PORT(pinName)];
    uint32_t pin = GPIO_EXTRACT_PIN(pinName);

    return PORT_Hal_IsPinIntPending(portBase, pin);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : GPIO_ClearPinIntFlag
 * Description   : Clear individual GPIO pin interrupt status flag.
 *
 *END**************************************************************************/
void GPIO_ClearPinIntFlag(uint32_t pinName)
{
    PORT_Type * portBase = g_portBase[GPIO_EXTRACT_PORT(pinName)];
    uint32_t pin = GPIO_EXTRACT_PIN(pinName);

    PORT_Hal_ClearPinIntFlag(portBase, pin);
}

/*******************************************************************************
 * EOF
 ******************************************************************************/

