
#ifndef __PORT_HAL_H__
#define __PORT_HAL_H__

#include "includes.h"

/*!
 * @addtogroup port_hal
 * @{
 */

/*!
 * @file
 *
 * The port features such as digital filter, pull, and so on are valid when
 * they are available in one of the pins. However, that doesn't mean that all pins have the
 * capabilities to use such features. Please see the related reference manual for
 * accurate pin features.
 */
#define PORT_INSTANCE_COUNT (5U) /*!< Number of instances of the PORT module. */
#define PORTA_IDX (0U) /*!< Instance number for PORTA. */
#define PORTB_IDX (1U) /*!< Instance number for PORTB. */
#define PORTC_IDX (2U) /*!< Instance number for PORTC. */
#define PORTD_IDX (3U) /*!< Instance number for PORTD. */
#define PORTE_IDX (4U) /*!< Instance number for PORTE. */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief Internal resistor pull feature selection*/

typedef enum port_pull {
    portPullDown = 0U,  /*!< Internal pull-down resistor is enabled. @internal gui name="Down"*/
    portPullUp   = 1U   /*!< Internal pull-up resistor is enabled. @internal gui name="Up"*/
}port_pull_t;

/*! @brief Slew rate selection*/
typedef enum port_slew_rate {
    portFastSlewRate = 0U,  /*!< Fast slew rate is configured. @internal gui name="Fast"*/
    portSlowSlewRate = 1U   /*!< Slow slew rate is configured. @internal gui name="Slow" */
}port_slew_rate_t;

/*! @brief Configures the drive strength.*/
typedef enum port_drive_strength {
    portLowDriveStrength  = 0U, /*!< Low drive strength is configured. @internal gui name="Low"*/
    portHighDriveStrength = 1U  /*!< High drive strength is configured. @internal gui name="High"*/
}port_drive_strength_t;

/*! @brief Pin mux selection*/
typedef enum port_mux {
    portPinDisabled = 0U,   /*!< Corresponding pin is disabled, but is used as an analog pin.*/
    portMuxAsGpio   = 1U,   /*!< Corresponding pin is configured as GPIO.*/
    portMuxAlt2     = 2U,   /*!< Chip-specific*/
    portMuxAlt3     = 3U,   /*!< Chip-specific*/
    portMuxAlt4     = 4U,   /*!< Chip-specific*/
    portMuxAlt5     = 5U,   /*!< Chip-specific*/
    portMuxAlt6     = 6U,   /*!< Chip-specific*/
    portMuxAlt7     = 7U    /*!< Chip-specific*/
}port_mux_t;

/*! @brief Configures the interrupt generation condition.*/
typedef enum _port_interrupt_config {
    portIntDisabled    = 0x0U,  /*!< Interrupt/DMA request is disabled.*/
    portDmaRisingEdge  = 0x1U,  /*!< DMA request on rising edge.*/
    portDmaFallingEdge = 0x2U,  /*!< DMA request on falling edge.*/
    portDmaEitherEdge  = 0x3U,  /*!< DMA request on either edge.*/
    portIntLogicZero   = 0x8U,  /*!< Interrupt when logic zero. */
    portIntRisingEdge  = 0x9U,  /*!< Interrupt on rising edge. */
    portIntFallingEdge = 0xAU,  /*!< Interrupt on falling edge. */
    portIntEitherEdge  = 0xBU,  /*!< Interrupt on either edge. */
    portIntLogicOne    = 0xCU   /*!< Interrupt when logic one. */
}port_interrupt_config_t;

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
 * @brief Selects the internal resistor as pull-down or pull-up.
 *
 * Pull configuration is valid in all digital pin muxing modes.
 *
 * @param base  port base pointer. PORTA/B/C...
 * @param pin       port pin number
 * @param pullSelect  internal resistor pull feature selection
 *        - kPortPullDown: internal pull-down resistor is enabled.
 *        - kPortPullUp  : internal pull-up resistor is enabled.
 */
__STATIC_INLINE void PORT_Hal_SetPullMode(PORT_Type * base, uint32_t pin, port_pull_t pullSelect)
{
    assert(pin < 32U);
	PORT_PCR_REG(base,pin) |= PORT_PCR_PS(pullSelect);
}

/*!
 * @brief Enables or disables the internal pull resistor.
 *
 * @param base  port base pointer
 * @param pin       port pin number
 * @param isPullEnabled  internal pull resistor enable or disable
 *        - true : internal pull resistor is enabled.
 *        - false: internal pull resistor is disabled.
 */
__STATIC_INLINE void PORT_Hal_SetPullCmd(PORT_Type * base, uint32_t pin, bool isPullEnabled)
{
    assert(pin < 32U);
    PORT_PCR_REG(base,pin) |= PORT_PCR_PE(isPullEnabled);
}

/*!
 * @brief Configures the fast/slow slew rate if the pin is used as a digital output.
 *
 * @param base  port base pointer
 * @param pin  port pin number
 * @param rateSelect  slew rate selection
 *        - kPortFastSlewRate: fast slew rate is configured.
 *        - kPortSlowSlewRate: slow slew rate is configured.
 */
__STATIC_INLINE void PORT_Hal_SetSlewRateMode(PORT_Type * base, uint32_t pin, port_slew_rate_t rateSelect)
{
    assert(pin < 32U);
    PORT_PCR_REG(base,pin) |= PORT_PCR_SRE(rateSelect);
}

/*!
 * @brief Configures the passive filter if the pin is used as a digital input.
 *
 * If enabled, a low pass filter (10 MHz to 30 MHz bandwidth)  is enabled
 * on the digital input path. Disable the Passive Input Filter when supporting
 * high speed interfaces (> 2 MHz) on the pin.
 *
 * @param base  port base pointer
 * @param pin  port pin number
 * @param isPassiveFilterEnabled  passive filter configuration
 *        - false: passive filter is disabled.
 *        - true : passive filter is enabled.
 */
__STATIC_INLINE void PORT_Hal_SetPassiveFilterCmd(PORT_Type * base, uint32_t pin, bool isPassiveFilterEnabled)
{
    assert(pin < 32U);
	PORT_PCR_REG(base,pin) |= PORT_PCR_PFE(isPassiveFilterEnabled);
}

/*!
 * @brief Configures the drive strength if the pin is used as a digital output.
 *
 * @param base  port base pointer
 * @param pin  port pin number
 * @param driveSelect  drive strength selection
 *        - kLowDriveStrength : low drive strength is configured.
 *        - kHighDriveStrength: high drive strength is configured.
 */
__STATIC_INLINE void PORT_Hal_SetDriveStrengthMode(PORT_Type * base, uint32_t pin, port_drive_strength_t driveSelect)
{
    assert(pin < 32U);
    PORT_PCR_REG(base,pin) |= PORT_PCR_DSE(driveSelect);
}


/*!
 * @brief Configures the pin muxing.
 *
 * @param base  port base pointer
 * @param pin  port pin number
 * @param mux  pin muxing slot selection
 *        - kPortPinDisabled: Pin disabled.
 *        - kPortMuxAsGpio  : Set as GPIO.
 *        - others          : chip-specific.
 */
__STATIC_INLINE void PORT_Hal_SetMuxMode(PORT_Type * base, uint32_t pin, port_mux_t mux)
{
    assert(pin < 32U);
    PORT_PCR_REG(base,pin) |= PORT_PCR_MUX(mux);
}


/*!
 * @brief Configures the low half of the pin control register for the same settings.
 *        This function operates pin 0 -15 of one specific port.
 *
 * @param base  port base pointer
 * @param lowPinSelect  update corresponding pin control register or not. For a specific bit:
 *        - 0: corresponding low half of pin control register won't be updated according to configuration.
 *        - 1: corresponding low half of pin control register will be updated according to configuration.
 * @param config  value  is written to a low half port control register bits[15:0].
 */
void PORT_Hal_SetLowGlobalPinCtrl(PORT_Type * base, uint16_t lowPinSelect, uint16_t config);

/*!
 * @brief Configures the high half of the pin control register for the same settings.
 *        This function operates pin 16 -31 of one specific port.
 *
 * @param base  port base pointer
 * @param highPinSelect  update corresponding pin control register or not. For a specific bit:
 *        - 0: corresponding high half of pin control register won't be updated according to configuration.
 *        - 1: corresponding high half of pin control register will be updated according to configuration.
 * @param config  value is  written to a high half port control register bits[15:0].
 */
void PORT_Hal_SetHighGlobalPinCtrl(PORT_Type * base, uint16_t highPinSelect, uint16_t config);

/*@}*/

/*!
 * @name Interrupt
 * @{
 */

/*!
 * @brief Configures the port pin interrupt/DMA request.
 *
 * @param base  port base pointer.
 * @param pin  port pin number
 * @param intConfig  interrupt configuration
 *        - kPortIntDisabled   : Interrupt/DMA request disabled.
 *        - kPortDmaRisingEdge : DMA request on rising edge.
 *        - kPortDmaFallingEdge: DMA request on falling edge.
 *        - kPortDmaEitherEdge : DMA request on either edge.
 *        - kPortIntLogicZero  : Interrupt when logic zero.
 *        - kPortIntRisingEdge : Interrupt on rising edge.
 *        - kPortIntFallingEdge: Interrupt on falling edge.
 *        - kPortIntEitherEdge : Interrupt on either edge.
 *        - kPortIntLogicOne   : Interrupt when logic one.
 */
__STATIC_INLINE void PORT_Hal_SetPinIntMode(PORT_Type * base, uint32_t pin, port_interrupt_config_t intConfig)
{
    assert(pin < 32U);
	PORT_PCR_REG(base,pin) |= PORT_PCR_IRQC(intConfig);
}

/*!
 * @brief Gets the current port pin interrupt/DMA request configuration.
 *
 * @param base  port base pointer
 * @param pin  port pin number
 * @return  interrupt configuration
 *        - kPortIntDisabled   : Interrupt/DMA request disabled.
 *        - kPortDmaRisingEdge : DMA request on rising edge.
 *        - kPortDmaFallingEdge: DMA request on falling edge.
 *        - kPortDmaEitherEdge : DMA request on either edge.
 *        - kPortIntLogicZero  : Interrupt when logic zero.
 *        - kPortIntRisingEdge : Interrupt on rising edge.
 *        - kPortIntFallingEdge: Interrupt on falling edge.
 *        - kPortIntEitherEdge : Interrupt on either edge.
 *        - kPortIntLogicOne   : Interrupt when logic one.
 */
__STATIC_INLINE port_interrupt_config_t PORT_Hal_GetPinIntMode(PORT_Type * base, uint32_t pin)
{
    assert(pin < 32U);
    return (port_interrupt_config_t)(PORT_PCR_REG(base,pin) & PORT_PCR_IRQC_MASK);
}

/*!
 * @brief Reads the individual pin-interrupt status flag.
 *
 * If a pin is configured to generate the DMA request,  the corresponding flag
 * is cleared automatically at the completion of the requested DMA transfer.
 * Otherwise, the flag remains set until a logic one is written to that flag.
 * If configured for a level sensitive interrupt that remains asserted, the flag
 * is set again immediately.
 *
 * @param base  port base pointer
 * @param pin  port pin number
 * @return current pin interrupt status flag
 *         - 0: interrupt is not detected.
 *         - 1: interrupt is detected.
 */
__STATIC_INLINE uint32_t PORT_Hal_IsPinIntPending(PORT_Type * base, uint32_t pin)
{
    assert(pin < 32U);
    return (PORT_PCR_REG(base,pin) & PORT_PCR_ISF_MASK);
}

/*!
 * @brief Clears the individual pin-interrupt status flag.
 *
 * @param base  port base pointer
 * @param pin  port pin number
 */
__STATIC_INLINE void PORT_Hal_ClearPinIntFlag(PORT_Type * base, uint32_t pin)
{
    assert(pin < 32U);
	PORT_PCR_REG(base,pin) |= PORT_PCR_ISF_MASK;
}

/*!
 * @brief Reads the entire port interrupt status flag.
 *
 * @param base  port base pointer
 * @return all 32 pin interrupt status flags. For specific bit:
 *         - 0: interrupt is not detected.
 *         - 1: interrupt is detected.
 */
__STATIC_INLINE uint32_t PORT_Hal_GetPortIntFlag(PORT_Type * base)
{
    return PORT_ISFR_REG(base);
}

/*!
 * @brief Clears the entire port interrupt status flag.
 *
 * @param base  port base pointer
 */
__STATIC_INLINE void PORT_Hal_ClearPortIntFlag(PORT_Type * base)
{
	PORT_ISFR_REG(base) = (uint32_t) ~0U;
}

/*@}*/

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* __PORT_HAL_H__*/
/*******************************************************************************
 * EOF
 ******************************************************************************/

