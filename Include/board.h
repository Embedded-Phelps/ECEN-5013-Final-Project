
#ifndef __BOARD_H__
#define __BOARD_H__

#include "includes.h"

/*! @file */
/*!*/
/*! This file contains gpio pin definitions used by gpio peripheral driver.*/
/*! The enums in _gpio_pins map to the real gpio pin numbers defined in*/
/*! gpioPinLookupTable. And this might be different in different board.*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief gpio pin names.*/
/*!*/
/*! This should be defined according to board setting.*/
enum _gpio_pins
{
    gpioLED1        = GPIO_MAKE_PIN(GPIOB_IDX, 19),   /* FRDM-KL25Z4 Green LED */
    gpioLED2        = GPIO_MAKE_PIN(GPIOB_IDX, 18),   /* FRDM-KL25Z4 Red LED */
    gpioLED3        = GPIO_MAKE_PIN(GPIOD_IDX,  1),   /* FRDM-KL25Z4 Blue LED */
    gpioSW1         = GPIO_MAKE_PIN(GPIOD_IDX,  0),   /* External switch */
};

extern gpio_input_pin_user_config_t switchPins[];
extern gpio_output_pin_user_config_t ledPins[];

/* The UART to use for debug messages. */
#ifndef BOARD_DEBUG_UART_INSTANCE
    #define BOARD_DEBUG_UART_INSTANCE   0
    #define BOARD_DEBUG_UART_BASEADDR   UART0
#endif
#ifndef BOARD_DEBUG_UART_BAUD
    #define BOARD_DEBUG_UART_BAUD       115200
#endif

/* This define to use for power manager demo */
#define BOARD_LOW_POWER_UART_BAUD       9600

#define BOARD_USE_LPSCI
#define PM_DBG_UART_IRQ_HANDLER         UART0_IRQHandler
#define PM_DBG_UART_IRQn                UART0_IRQn

#define BOARD_I2C_GPIO_SCL              GPIO_MAKE_PIN(GPIOE_IDX, 24)
#define BOARD_I2C_GPIO_SDA              GPIO_MAKE_PIN(GPIOE_IDX, 25)

/* The i2c instance used for i2c connection by default */
#define BOARD_I2C_INSTANCE              1

/* The bubble level demo information */
#define BOARD_BUBBLE_TPM_INSTANCE       2
#define BOARD_TPM_X_CHANNEL             0
#define BOARD_TPM_Y_CHANNEL             1
#define BOARD_MMA8451_ADDR              0x1D
#define BOARD_ACCEL_ADDR                BOARD_MMA8451_ADDR
#define BOARD_ACCEL_BAUDRATE            100
#define BOARD_ACCEL_I2C_INSTANCE        0

/* board led color mapping */
#define BOARD_GPIO_LED_BLUE             gpioLED3
#define BOARD_GPIO_LED_RED              gpioLED2
#define BOARD_GPIO_LED_GREEN            gpioLED1


#define BOARD_TSI_ELECTRODE_CNT         2
#define BOARD_TSI_ELECTRODE_1           9
#define BOARD_TSI_ELECTRODE_2           10

#define LED1_EN (GPIO_OutputPinInit(&ledPins[0])) 	/*!< Enable target LED1 */
#define LED2_EN (GPIO_OutputPinInit(&ledPins[1])) 	/*!< Enable target LED2 */
#define LED3_EN (GPIO_OutputPinInit(&ledPins[2])) 	/*!< Enable target LED3 */

#define LED1_SELECT (PORT_Hal_SetMuxMode(PORTB, 19, portMuxAsGpio))    /*!< Enable target LED1 */
#define LED2_SELECT (PORT_Hal_SetMuxMode(PORTB, 18, portMuxAsGpio)) 	 /*!< Enable target LED2 */
#define LED3_SELECT (PORT_Hal_SetMuxMode(PORTD,  1, portMuxAsGpio)) 	 /*!< Enable target LED3 */

#define LED1_OFF (GPIO_WritePinOutput(ledPins[0].pinName, 1))         /*!< Turn off target LED1 */
#define LED2_OFF (GPIO_WritePinOutput(ledPins[1].pinName, 1))         /*!< Turn off target LED2 */
#define LED3_OFF (GPIO_WritePinOutput(ledPins[2].pinName, 1))         /*!< Turn off target LED3 */

#define LED1_ON (GPIO_WritePinOutput(ledPins[0].pinName, 0))          /*!< Turn on target LED1 */
#define LED2_ON (GPIO_WritePinOutput(ledPins[1].pinName, 0))          /*!< Turn on target LED2 */
#define LED3_ON (GPIO_WritePinOutput(ledPins[2].pinName, 0))          /*!< Turn on target LED3 */

#define LED1_TOGGLE (GPIO_TogglePinOutput(ledPins[0].pinName))          /*!< Toggle on target LED1 */
#define LED2_TOGGLE (GPIO_TogglePinOutput(ledPins[1].pinName))          /*!< Toggle on target LED2 */
#define LED3_TOGGLE (GPIO_TogglePinOutput(ledPins[2].pinName))          /*!< Toggle on target LED3 */

#define SW1_SELECT (PORT_Hal_SetMuxMode(PORTD, 0, portMuxAsGpio))
#define SW1_EN	   (GPIO_InputPinInit(&switchPins[0]))

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */



#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* __BOARD_H__ */


