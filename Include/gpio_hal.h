
#ifndef __GPIO_HAL_H__
#define __GPIO_HAL_H__
 
#include "includes.h"
 
typedef enum gpio_pin_direction {
    gpioDigitalInput  = 0U, /*!< Set current pin as digital input*/
    gpioDigitalOutput = 1U  /*!< Set current pin as digital output*/
}gpio_pin_direction_t;

/*******************************************************************************
 * API
 ******************************************************************************/
 
#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name Configuration
 * @{
 */

/*!
 * @brief Sets the individual GPIO pin to general input or output.
 * 
 * @param base  GPIO base pointer(PTA, PTB, PTC, etc.) 
 * @param pin  GPIO port pin number 
 * @param direction  GPIO directions
 *        - kGpioDigitalInput: set to input
 *        - kGpioDigitalOutput: set to output
 */
void GPIO_Hal_SetPinDir(GPIO_Type * base, uint32_t pin,
                        gpio_pin_direction_t direction);

/*!
 * @brief Sets the GPIO port pins to general input or output.
 *
 * This function  operates all 32 port pins.
 * 
 * @param base  GPIO base pointer (PTA, PTB, PTC, etc.)
 * @param pinDirectionMap  GPIO directions bit map
 *        - 0: set to input
 *        - 1: set to output
 *        - LSB: pin 0
 *        - MSB: pin 31
 */
__STATIC_INLINE void GPIO_Hal_SetPortDir(GPIO_Type * base, uint32_t pinDirectionMap)
{
	GPIO_PDDR_REG(base) = pinDirectionMap;
}

/* @} */

/*!
 * @name Status
 * @{
 */

/*!
 * @brief Gets the current direction of the individual GPIO pin.
 * 
 * @param base  GPIO base pointer(PTA, PTB, PTC, etc.)
 * @param pin  GPIO port pin number
 * @return GPIO directions
 *        - kGpioDigitalInput: corresponding pin is set to input.
 *        - kGpioDigitalOutput: corresponding pin is set to output.
 */
__STATIC_INLINE gpio_pin_direction_t GPIO_Hal_GetPinDir(GPIO_Type * base, uint32_t pin)
{
    assert(pin < 32);
    return (gpio_pin_direction_t)((GPIO_PDDR_REG(base) >> pin) & 1U);
} 

/*!
 * @brief Gets the GPIO port pins direction.
 *
 * This function  gets all 32-pin directions as a 32-bit integer.
 * 
 * @param base  GPIO base pointer (PTA, PTB, PTC, etc.)
 * @return GPIO directions. Each bit represents one pin. For each bit:
 *        - 0: corresponding pin is set to input
 *        - 1: corresponding pin is set to output
 *        - LSB: pin 0
 *        - MSB: pin 31
 */
__STATIC_INLINE uint32_t GPIO_Hal_GetPortDir(GPIO_Type * base)
{
    return GPIO_PDDR_REG(base);
} 

/* @} */

/*!
 * @name Output Operation
 * @{
 */

/*!
 * @brief Sets the output level of the individual GPIO pin to logic 1 or 0.
 * 
 * @param base  GPIO base pointer(PTA, PTB, PTC, etc.)
 * @param pin  GPIO port pin number
 * @param output  pin output logic level
 */
void GPIO_Hal_WritePinOutput(GPIO_Type * base, uint32_t pin, bool output);

/*!
 * @brief Reads the current pin output.
 * 
 * @param base  GPIO base pointer (PTA, PTB, PTC, etc.)
 * @param pin  GPIO port pin number
 * @return current pin output status. 0 - Low logic, 1 - High logic
 */
__STATIC_INLINE uint32_t GPIO_Hal_ReadPinOutput(GPIO_Type * base, uint32_t pin)
{
    assert(pin < 32);
    return ((GPIO_PDOR_REG(base) >> pin) & 0x1U);
}

/*!
 * @brief Sets the output level of the individual GPIO pin to logic 1.
 * 
 * @param base  GPIO base pointer(PTA, PTB, PTC, etc.)
 * @param pin  GPIO port pin number
 */
__STATIC_INLINE void GPIO_Hal_SetPinOutput(GPIO_Type * base, uint32_t pin)
{
    assert(pin < 32);
    GPIO_PSOR_REG(base) |= (1U << pin);
}

/*!
 * @brief Clears the output level of the individual GPIO pin to logic 0.
 * 
 * @param base  GPIO base pointer(PTA, PTB, PTC, etc.)
 * @param pin  GPIO port pin number
 */
__STATIC_INLINE void GPIO_Hal_ClearPinOutput(GPIO_Type * base, uint32_t pin)
{
    assert(pin < 32);
    GPIO_PCOR_REG(base) |= (1U << pin);
}

/*!
 * @brief Reverses the current output logic of the individual GPIO pin.
 * 
 * @param base  GPIO base pointer(PTA, PTB, PTC, etc.)
 * @param pin  GPIO port pin number
 */
__STATIC_INLINE void GPIO_Hal_TogglePinOutput(GPIO_Type * base, uint32_t pin)
{
    assert(pin < 32);
    GPIO_PTOR_REG(base) = (1U << pin);
}

/*!
 * @brief Sets the output of the GPIO port pins to a specific logic value.
 *
 * This function  operates all 32 port pins.
 * 
 * @param base  GPIO base pointer (PTA, PTB, PTC, etc.) 
 * @param portOutput  data to configure the GPIO output. Each bit represents one pin. For each bit:
 *        - 0: set logic level 0 to pin
 *        - 1: set logic level 1 to pin
 *        - LSB: pin 0
 *        - MSB: pin 31
 */
__STATIC_INLINE void GPIO_Hal_WritePortOutput(GPIO_Type * base, uint32_t portOutput)
{
    GPIO_PDOR_REG(base) = portOutput;
}

/*!
 * @brief Reads out all pin output status of the current port.
 *
 * This function  operates all 32 port pins.
 * 
 * @param base  GPIO base pointer (PTA, PTB, PTC, etc.) 
 * @return current port output status. Each bit represents one pin. For each bit:
 *        - 0: corresponding pin is outputting logic level 0
 *        - 1: corresponding pin is outputting logic level 1
 *        - LSB: pin 0
 *        - MSB: pin 31
 */
__STATIC_INLINE uint32_t GPIO_Hal_ReadPortOutput(GPIO_Type * base)
{
    return GPIO_PDOR_REG(base);
}

/*!
 * @brief Sets the output level of the GPIO port pins to logic 1.
 *
 * This function  operates all 32 port pins.
 *
 * @param base  GPIO base pointer(PTA, PTB, PTC, etc.)
 * @param portOutput GPIO output port pin mask. Each bit represents one pin. For each bit:
 *        - 0: pin output will not be changed.
 *        - 1: pin output will be set to logic level 1
 *        - LSB: pin 0
 *        - MSB: pin 31
 */
__STATIC_INLINE void GPIO_Hal_SetPortOutput(GPIO_Type * base, uint32_t portOutput)
{
    GPIO_PSOR_REG(base) = portOutput;
}

/*!
 * @brief Clears the output level of the GPIO port pins to logic 0.
 * 
 * This function  operates all 32 port pins.
 *
 * @param base  GPIO base pointer(PTA, PTB, PTC, etc.)
 * @param portOutput  mask of GPIO output pins. Each bit represents one pin. For each bit:
 *        - 0: pin output will not be changed.
 *        - 1: pin output will be set to logic level 0
 *        - LSB: pin 0
 *        - MSB: pin 31
 */
__STATIC_INLINE void GPIO_Hal_ClearPortOutput(GPIO_Type * base, uint32_t portOutput)
{
    GPIO_PCOR_REG(base) = portOutput;
}

/*!
 * @brief Reverses the current output logic of the GPIO port pins.
 * 
 * This function  operates all 32 port pins.
 *
 * @param base  GPIO base pointer(PTA, PTB, PTC, etc.)
 * @param portOutput  mask of GPIO output pins. Each bit represents one pin. For each bit:
 *        - 0: pin output will not be changed.
 *        - 1: pin output logic level will be reversed.
 *        - LSB: pin 0
 *        - MSB: pin 31
 */
__STATIC_INLINE void GPIO_Hal_TogglePortOutput(GPIO_Type * base, uint32_t portOutput)
{
    GPIO_PTOR_REG(base) = portOutput;
}

/* @} */

/*!
 * @name Input Operation
 * @{
 */

/*!
 * @brief Reads the current input value of the individual GPIO pin.
 * 
 * @param base  GPIO base pointer(PTA, PTB, PTC, etc.)
 * @param pin  GPIO port pin number
 * @return GPIO port input value
 *         - 0: Pin logic level is 0, or is not configured for use by digital function.
 *         - 1: Pin logic level is 1
 */
__STATIC_INLINE uint32_t GPIO_Hal_ReadPinInput(GPIO_Type * base, uint32_t pin)
{
    assert(pin < 32);
    return ((GPIO_PDIR_REG(base) >> pin) & 1U);
}

/*!
 * @brief Reads the current input value of a specific GPIO port.
 *
 * This function  gets all 32-pin input as a 32-bit integer.
 * 
 * @param base GPIO base pointer(PTA, PTB, PTC, etc.)
 * @return GPIO port input data. Each bit represents one pin. For each bit:
 *         - 0: Pin logic level is 0, or is not configured for use by digital function.
 *         - 1: Pin logic level is 1.
 *         - LSB: pin 0
 *         - MSB: pin 31
 */
__STATIC_INLINE uint32_t GPIO_Hal_ReadPortInput(GPIO_Type * base)
{
    return GPIO_PDIR_REG(base);
}

/* @} */

/*!
 * @name FGPIO Operation
 *
 * @note FGPIO (Fast GPIO) is only available in a few MCUs. FGPIO and GPIO share the same
 *       peripheral but use different registers. FGPIO is closer to the core than the regular GPIO
 *       and it's faster to read and write.
 * @{
 */

/*!
 * @name Configuration
 * @{
 */

/*!
 * @brief Sets the individual FGPIO pin to general input or output.
 *
 * @param base FGPIO base pointer(FPTA, FPTB, FPTC, etc.) 
 * @param pin  FGPIO port pin number 
 * @param direction  FGPIO directions
 *        - kGpioDigitalInput: set to input
 *        - kGpioDigitalOutput: set to output
 */
void FGPIO_Hal_SetPinDir(FGPIO_Type * base, uint32_t pin, gpio_pin_direction_t direction);

/*!
 * @brief Sets the FGPIO port pins to general input or output.
 *
 * This function  operates all 32 port pins.
 *
 * @param base  FGPIO base pointer (FPTA, FPTB, FPTC, etc.)
 * @param pinDirectionMap  FGPIO directions bit map
 *        - 0: set to input
 *        - 1: set to output
 *        - LSB: pin 0
 *        - MSB: pin 31
 */
__STATIC_INLINE void FGPIO_Hal_SetPortDir(FGPIO_Type * base, uint32_t pinDirectionMap)
{
    FGPIO_PDDR_REG(base) = pinDirectionMap;
}

/* @} */

/*!
 * @name Status
 * @{
 */

/*!
 * @brief Gets the current direction of the individual FGPIO pin.
 * 
 * @param base  FGPIO base pointer(FPTA, FPTB, FPTC, etc.)
 * @param pin  FGPIO port pin number
 * @return FGPIO directions
 *        - kGpioDigitalInput: corresponding pin is set to input.
 *        - kGpioDigitalOutput: corresponding pin is set to output.
 */
__STATIC_INLINE gpio_pin_direction_t FGPIO_Hal_GetPinDir(FGPIO_Type * base, uint32_t pin)
{
    assert(pin < 32);
    return (gpio_pin_direction_t)((FGPIO_PDDR_REG(base) >> pin) & 1U);
} 

/*!
 * @brief Gets the FGPIO port pins direction.
 *
 * This function  gets all 32-pin directions as a 32-bit integer.
 * 
 * @param base  FGPIO base pointer (FPTA, FPTB, FPTC, etc.)
 * @return FGPIO directions. Each bit represents one pin. For each bit:
 *        - 0: corresponding pin is set to input
 *        - 1: corresponding pin is set to output
 *        - LSB: pin 0
 *        - MSB: pin 31
 */
__STATIC_INLINE uint32_t FGPIO_Hal_GetPortDir(FGPIO_Type * base)
{
    return FGPIO_PDDR_REG(base);
}

/* @} */

/*!
 * @name Output Operation
 * @{
 */

/*!
 * @brief Sets the output level of the individual FGPIO pin to logic 1 or 0.
 * 
 * @param base  FGPIO base pointer(FPTA, FPTB, FPTC, etc.)
 * @param pin  FGPIO port pin number
 * @param output  pin output logic level
 */
void FGPIO_Hal_WritePinOutput(FGPIO_Type * base, uint32_t pin, bool output);

/*!
 * @brief Reads the current FGPIOpin output.
 * 
 * @param base  FGPIO base pointer (FPTA, FPTB, FPTC, etc.)
 * @param pin  FGPIO port pin number
 * @return current pin output status. 0 - Low logic, 1 - High logic
 */
__STATIC_INLINE uint32_t FGPIO_Hal_ReadPinOutput(FGPIO_Type * base, uint32_t pin)
{
    assert(pin < 32);
    return ((FGPIO_PDOR_REG(base) >> pin) & 0x1U);
}

/*!
 * @brief Sets the output level of an individual FGPIO pin to logic 1.
 * 
 * @param base  GPIO base pointer(FPTA, FPTB, FPTC, etc.)
 * @param pin  FGPIO port pin number
 */
__STATIC_INLINE void FGPIO_Hal_SetPinOutput(FGPIO_Type * base, uint32_t pin)
{
    assert(pin < 32);
    FGPIO_PSOR_REG(base) |= (1U << pin);
}

/*!
 * @brief Clears the output level of an individual FGPIO pin to logic 0.
 * 
 * @param base  GPIO base pointer(FPTA, FPTB, FPTC, etc.)
 * @param pin  FGPIO port pin number
 */
__STATIC_INLINE void FGPIO_Hal_ClearPinOutput(FGPIO_Type * base, uint32_t pin)
{
    assert(pin < 32);
    FGPIO_PCOR_REG(base) |= (1U << pin);
}

/*!
 * @brief Reverses the current output logic of an individual FGPIO pin.
 * 
 * @param base  GPIO base pointer(FPTA, FPTB, FPTC, etc.)
 * @param pin  FGPIO port pin number
 */
__STATIC_INLINE void FGPIO_Hal_TogglePinOutput(FGPIO_Type * base, uint32_t pin)
{
    assert(pin < 32);
    FGPIO_PTOR_REG(base) |= (1U << pin);
}

/*!
 * @brief Sets the output of the FGPIO port pins to a specific logic value. 
 *
 * This function  affects all 32 port pins.
 * 
 * @param base  GPIO base pointer(FPTA, FPTB, FPTC, etc.)
 * @param portOutput  data to configure the GPIO output. Each bit represents one pin. For each bit:
 *        - 0: set logic level 0 to pin.
 *        - 1: set logic level 1 to pin.
 *        - LSB: pin 0
 *        - MSB: pin 31
 */
__STATIC_INLINE void FGPIO_Hal_WritePortOutput(FGPIO_Type * base, uint32_t portOutput)
{
    FGPIO_PDOR_REG(base) = portOutput;
}

/*!
 * @brief Reads out all pin output status of the current port.
 *
 * This function  operates all 32 port pins.
 * 
 * @param base  FGPIO base pointer (FPTA, FPTB, FPTC, etc.) 
 * @return current port output status. Each bit represents one pin. For each bit:
 *        - 0: corresponding pin is outputting logic level 0
 *        - 1: corresponding pin is outputting logic level 1
 *        - LSB: pin 0
 *        - MSB: pin 31
 */
__STATIC_INLINE uint32_t FGPIO_Hal_ReadPortOutput(FGPIO_Type * base)
{
    return FGPIO_PDOR_REG(base);
}

/*!
 * @brief Sets the output level of the FGPIO port pins to logic 1.
 *
 * This function  affects all 32 port pins.
 *
 * @param base  FGPIO base pointer(FPTA, FPTB, FPTC, etc.)
 * @param portOutput  mask of FGPIO output pins. Each bit represents one pin. For each bit:
 *        - 0: pin output will not be changed.
 *        - 1: pin output will be set to logic level 1
 *        - LSB: pin 0
 *        - MSB: pin 31
 */
__STATIC_INLINE void FGPIO_Hal_SetPortOutput(FGPIO_Type * base, uint32_t portOutput)
{
    FGPIO_PSOR_REG(base) = portOutput;
}

/*!
 * @brief Clears the output level of the FGPIO port pins to logic 0.
 *
 * This function  affects all 32 port pins.
 *
 * @param base  FGPIO base pointer(FPTA, FPTB, FPTC, etc.)
 * @param portOutput  mask of FGPIO output pins. Each bit represents one pin. For each bit:
 *        - 0: pin output will not be changed.
 *        - 1: pin output will be set to logic level 0
 *        - LSB: pin 0
 *        - MSB: pin 31
 */
__STATIC_INLINE void FGPIO_Hal_ClearPortOutput(FGPIO_Type * base, uint32_t portOutput)
{
    FGPIO_PCOR_REG(base) = portOutput;
}

/*!
 * @brief Reverses the current output logic of the FGPIO port pins.
 *
 * This function  affects all 32 port pins.
 * 
 * @param base  FGPIO base pointer(FPTA, FPTB, FPTC, etc.)
 * @param portOutput  mask of FGPIO output pins. Each bit represents one pin. For each bit:
 *        - 0: pin output will not be changed.
 *        - 1: pin output logic level will be reversed.
 *        - LSB: pin 0
 *        - MSB: pin 31
 */
__STATIC_INLINE void FGPIO_Hal_TogglePortOutput(FGPIO_Type * base, uint32_t portOutput)
{
    FGPIO_PTOR_REG(base) = portOutput;
}

/* @} */

/*!
 * @name Input Operation
 * @{ 
 */

/*!
 * @brief Gets the current input value of an individual FGPIO pin.
 * 
 * @param base  GPIO base pointer(FPTA, FPTB, FPTC, etc.)
 * @param pin  FGPIO port pin number
 * @return FGPIO port input data
 *         - 0: Pin logic level is 0, or is not configured for use by digital function.
 *         - 1: Pin logic level is 1.
 */
__STATIC_INLINE uint32_t FGPIO_Hal_ReadPinInput(FGPIO_Type * base, uint32_t pin)
{
    assert(pin < 32);
    return (FGPIO_PDIR_REG(base) >> pin) & 1U;
}

/*!
 * @brief Gets the current input value of a specific FGPIO port.
 *
 * This function  gets all 32-pin input as a 32-bit integer.
 * 
 * @param base  GPIO base pointer(FPTA, FPTB, FPTC, etc.). 
 * @return FGPIO port input data. Each bit represents one pin. For each bit:
 *         - 0: Pin logic level is 0, or is not configured for use by digital function.
 *         - 1: Pin logic level is 1.
 *         - LSB: pin 0
 *         - MSB: pin 31
 */
__STATIC_INLINE uint32_t FGPIO_Hal_ReadPortInput(FGPIO_Type * base)
{
    return FGPIO_PDIR_REG(base);
}

/* @} */

#if defined(__cplusplus)
}
#endif
 
/*! @} */
 
#endif /* __GPIO_Hal_H__*/
/*******************************************************************************
 * EOF
 ******************************************************************************/
