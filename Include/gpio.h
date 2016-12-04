
#ifndef __GPIO_H__
#define __GPIO_H__

#include "includes.h"

/*!
 * @addtogroup gpio_driver
 * @{
 */

/*!
 * @file
 *
 * The GPIO driver uses the virtual GPIO name rather than an actual port and a
 * pin number. By using the virtual name, each pin name is self-explanatory.
 * To use the GPIO driver, an enumeration variable must be predefined in the
 * user application files. The variable saves all GPIO pin information used
 * in a project.
 *
 * This example shows how to define the enumeration variable.
   @code
   // This is the enumeration to define virtual GPIO pin names.
   // These members are used by "uint32_t pinName" in
   // gpio_output_pin_user_config_t
   // and gpio_input_pin_user_config_t. Usually defined in a header file.
   enum _gpio_pins
   {
       kGpioLED1  = GPIO_MAKE_PIN(GPIOA_IDX, 5), // Orange LED.
       kGpioLED2  = GPIO_MAKE_PIN(GPIOA_IDX, 6), // Yellow LED.
       kGpioLED3  = GPIO_MAKE_PIN(GPIOA_IDX, 7), // Green LED.
       kGpioLED4  = GPIO_MAKE_PIN(GPIOB_IDX, 8), // Red LED.
   };
   @endcode
 *
 * The port features such as "digital filter", "pull", are valid when
 * they are available in one of the pins. That doesn't mean, however, that all pins have the
 * capability to use such features. See the related reference manual for
 * accurate pin features.
 */

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief Table of base addresses for GPIO instances. */
extern GPIO_Type * const g_gpioBase[GPIO_INSTANCE_COUNT];

/*! @brief Table of base addresses for PORT instances. */
extern PORT_Type * const g_portBase[PORT_INSTANCE_COUNT];

/* Table to save PORT IRQ enumeration numbers defined in CMSIS header file */
extern const IRQn_Type g_portIrqId[PORT_INSTANCE_COUNT];

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
 * @name GPIO Pin Macros
 * @{
 */

#define GPIO_INSTANCE_COUNT (5U) /*!< Number of instances of the GPIO module. */
#define GPIOA_IDX (0U) /*!< Instance number for GPIOA. */
#define GPIOB_IDX (1U) /*!< Instance number for GPIOB. */
#define GPIOC_IDX (2U) /*!< Instance number for GPIOC. */
#define GPIOD_IDX (3U) /*!< Instance number for GPIOD. */
#define GPIOE_IDX (4U) /*!< Instance number for GPIOE. */
/*! @brief Indicates the end of a pin configuration structure.*/
#define GPIO_PINS_OUT_OF_RANGE (0xFFFFFFFFU)

/*! @brief Bits shifted for the GPIO port number. */
#define GPIO_PORT_SHIFT (0x8U)

/*! @brief Combines the port number and the pin number into a single scalar value. */
#define GPIO_MAKE_PIN(r,p) (((r)<< GPIO_PORT_SHIFT) | (p))

/*! @brief Extracts the port number from a combined port and pin value.*/
#define GPIO_EXTRACT_PORT(v) (((v) >> GPIO_PORT_SHIFT) & 0xFFU)

/*! @brief Extracts the pin number from a combined port and pin value.*/
#define GPIO_EXTRACT_PIN(v) ((v) & 0xFFU)

/* @} */

/*!
 * @brief The GPIO input pin configuration structure.
 *
 * Although every pin is configurable, valid configurations depend on a specific
 * device. Users should check the related reference manual to ensure that the
 * specific feature is valid in an individual pin. A configuration of
 * unavailable features is harmless, but takes no effect.
 */
typedef struct GpioInputPin {
    bool isPullEnable;                  /*!< Enable or disable pull. */
    port_pull_t pullSelect;             /*!< Select internal pull(up/down) resistor.*/
    bool isPassiveFilterEnabled;        /*!< Enable or disable passive filter.*/
    port_interrupt_config_t interrupt;  /*!< Select interrupt/DMA request.*/
} gpio_input_pin_t;

/*!
 * @brief The GPIO output pin configuration structure.
 *
 * Although every pin is configurable, valid configurations
 * depend on a specific device. Users should check the related reference manual to
 * ensure that the specific feature is valid in an individual pin. The configuration of
 * unavailable features is harmless, but takes no effect.
 */
typedef struct GpioOutputPin {
    uint32_t outputLogic;               /*!< Set default output logic.*/
    port_slew_rate_t slewRate;          /*! Select fast/slow slew rate.*/
    port_drive_strength_t driveStrength;/*!< Select low/high drive strength.*/
} gpio_output_pin_t;

/*!
 * @brief The GPIO input pin structure.
 *
 * Although the pinName is defined as a uint32_t type, values assigned to the pinName
 * should be the enumeration names defined in the enum _gpio_pins.
 */
typedef struct GpioInputPinUserConfig {
    uint32_t pinName;        /*!< Virtual pin name from enumeration defined by the user.*/
    gpio_input_pin_t config; /*!< Input pin configuration structure.*/
} gpio_input_pin_user_config_t;

/*!
 * @brief The GPIO output pin structure.
 *
 * Although the pinName is defined as a uint32_t type, values assigned to the pinName
 * should be the enumeration names defined in the enum _gpio_pins.
 */
typedef struct GpioOutputPinUserConfig {
    uint32_t pinName;        /*!< Virtual pin name from enumeration defined by the user.*/
    gpio_output_pin_t config;/*!< Input pin configuration structure.*/
} gpio_output_pin_user_config_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name Initialization
 * @{
 */

/*!
 * @brief Initializes all GPIO pins used by the board.
 *
 * To initialize the GPIO driver, define two arrays similar to the gpio_input_pin_user_config_t
 * inputPin[] array and the gpio_output_pin_user_config_t outputPin[] array in the user file.
 * Then, call the GPIO_DRV_Init() function and pass in the two arrays. If the input or output
 * pins are not needed, pass in a NULL.
 *
 * This is an example to define an input pin array:
   @code
   // Configure the kGpioPTA2 as digital input.
   gpio_input_pin_user_config_t inputPin[] = {
     {
         .pinName = kGpioPTA2,
         .config.isPullEnable = false,
         .config.pullSelect = kPortPullDown,
         .config.isPassiveFilterEnabled = false,
         .config.interrupt = kPortIntDisabled,
     },
     {
        // Note: This pinName must be defined here to indicate the end of the array.
        .pinName = GPIO_PINS_OUT_OF_RANGE,
     }
   };
   @endcode
 *
 * @param inputPins input GPIO pins pointer.
 * @param outputPins output GPIO pins pointer.
 */
void GPIO_Init(const gpio_input_pin_user_config_t * inputPins, const gpio_output_pin_user_config_t * outputPins);

/*!
 * @brief Initializes one GPIO input pin used by the board.
 *
 * @param inputPin input GPIO pins pointer.
 */
void GPIO_InputPinInit(const gpio_input_pin_user_config_t *inputPin);

/*!
 * @brief Initializes one GPIO output pin used by the board.
 *
 * @param outputPin output GPIO pins pointer.
 */
void GPIO_OutputPinInit(const gpio_output_pin_user_config_t *outputPin);

/* @} */

/*!
 * @name Pin Direction
 * @{
 */

/*!
 * @brief Gets the current direction of the individual GPIO pin.
 *
 * @param pinName GPIO pin name defined by the user in the GPIO pin enumeration list.
 * @return GPIO directions.
 *         - kGpioDigitalInput: corresponding pin is set as digital input.
 *         - kGpioDigitalOutput: corresponding pin is set as digital output.
 */
gpio_pin_direction_t GPIO_GetPinDir(uint32_t pinName);

/*!
 * @brief Sets the current direction of the individual GPIO pin.
 *
 * @param pinName GPIO pin name defined by the user in the GPIO pin enumeration list.
 * @param direction GPIO directions.
 *         - kGpioDigitalInput: corresponding pin is set as digital input.
 *         - kGpioDigitalOutput: corresponding pin is set as digital output.
 */

void GPIO_SetPinDir(uint32_t pinName, gpio_pin_direction_t direction);
/* @} */

/*!
 * @name Output Operations
 * @{
 */

/*!
 * @brief Sets the output level of the individual GPIO pin to the logic 1 or 0.
 *
 * @param pinName GPIO pin name defined by the user in the GPIO pin enumeration list.
 * @param output  pin output logic level.
 *        - 0: corresponding pin output low logic level.
 *        - Non-0: corresponding pin output high logic level.
 */
void GPIO_WritePinOutput(uint32_t pinName, uint32_t output);

/*!
 * @brief Sets the output level of the individual GPIO pin to the logic 1.
 *
 * @param pinName GPIO pin name defined by the user in the GPIO pin enumeration list.
 */
void GPIO_SetPinOutput(uint32_t pinName);

/*!
 * @brief Sets the output level of the individual GPIO pin to the logic 0.
 *
 * @param pinName GPIO pin name defined by the user in the GPIO pin enumeration list.
 */
void GPIO_ClearPinOutput(uint32_t pinName);

/*!
 * @brief Reverses current output logic of the individual GPIO pin.
 *
 * @param pinName GPIO pin name defined by the user in the GPIO pin enumeration list.
 */
void GPIO_TogglePinOutput(uint32_t pinName);

/* @} */

/*!
 * @name Input Operations
 * @{
 */

/*!
 * @brief Reads the current input value of the individual GPIO pin.
 *
 * @param pinName GPIO pin name defined by the user in the GPIO pin enumeration list.
 * @return GPIO port input value.
 *         - 0: Pin logic level is 0, or is not configured for use by digital function.
 *         - 1: Pin logic level is 1.
 */
uint32_t GPIO_ReadPinInput(uint32_t pinName);

/* @} */

/*!
 * @name Interrupt
 * @{
 */

/*!
 * @brief Reads the individual pin-interrupt status flag.
 *
 * If a pin is configured to generate the DMA request,  the corresponding flag
 * is cleared automatically at the completion of the requested DMA transfer.
 * Otherwise, the flag remains set until a logic one is written to that flag.
 * If configured for a level sensitive interrupt that remains asserted, the flag
 * is set again immediately.
 *
 * @param pinName GPIO pin name defined by the user in the GPIO pin enumeration list.
 * @return current pin interrupt status flag
 *         - 0: interrupt is not detected.
 *         - 1: interrupt is detected.
 */
uint32_t GPIO_IsPinIntPending(uint32_t pinName);

/*!
 * @brief Clears the individual GPIO pin interrupt status flag.
 *
 * @param pinName GPIO pin name defined by the user in the GPIO pin enumeration list.
 */
void GPIO_ClearPinIntFlag(uint32_t pinName);

/* @} */

#if defined(__cplusplus)
}
#endif

/*! @} */

#endif /* __GPIO_H__*/
/*******************************************************************************
 * EOF
 ******************************************************************************/

