
#include "inlcudes.h"


/*******************************************************************************
 * Code
 ******************************************************************************/
/*FUNCTION**********************************************************************
 *
 * Function Name : GPIO_Hal_SetPinDir
 * Description   : Set individual gpio pin to general input or output.
 *
 *END**************************************************************************/
void GPIO_Hal_SetPinDir(GPIO_Type * base, uint32_t pin, gpio_pin_direction_t direction)
{
    assert(pin < 32);

    if (direction == gpioDigitalOutput)
    {
        GPIO_PDDR_REG(base) |= (1U << pin);
    }
    else
    {
        GPIO_PDDR_REG(base) &= ~(1U << pin);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : GPIO_Hal_WritePinOutput
 * Description   : Set output level of individual gpio pin to logic 1 or 0.
 *
 *END**************************************************************************/
void GPIO_Hal_WritePinOutput(GPIO_Type * base, uint32_t pin, bool output)
{
    assert(pin < 32);

    if (output)
    {
        GPIO_PSOR_REG(base) |= (1U << pin); /* Set pin output to high level.*/
    }
    else
    {
        GPIO_PCOR_REG(base) |= (1U << pin); /* Set pin output to low level.*/
    }
}

#if FSL_FEATURE_GPIO_HAS_FAST_GPIO

/*FUNCTION**********************************************************************
 *
 * Function Name : FGPIO_Hal_SetPinDir
 * Description   : Set individual gpio pin to general input or output.
 *
 *END**************************************************************************/
void FGPIO_Hal_SetPinDir(FGPIO_Type * base, uint32_t pin, gpio_pin_direction_t direction)
{
    if (direction == kGpioDigitalOutput)
    {
        FGPIO_PDDR_REG(base) |= (1U << pin);
    }
    else
    {
        FGPIO_PDDR_REG(base) &= ~(1U << pin);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FGPIO_Hal_WritePinOutput
 * Description   : Set output level of individual gpio pin to logic 1 or 0.
 *
 *END**************************************************************************/
void FGPIO_Hal_WritePinOutput(FGPIO_Type * base, uint32_t pin, bool output)
{
    if (output)
    {
        FGPIO_PSOR_REG(base) |= (1U << pin); /* Set pin output to high level.*/
    }
    else
    {
        FGPIO_PCOR_REG(base) |= (1U << pin); /* Set pin output to low level.*/
    }
}

#endif

#endif /* FSL_FEATURE_SOC_GPIO_COUNT */
/*******************************************************************************
 * EOF
 ******************************************************************************/
