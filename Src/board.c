
#include "includes.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

gpio_input_pin_user_config_t switchPins[] = {
    {
        .pinName = gpioSW1,
        .config.isPullEnable = true,
        .config.isPassiveFilterEnabled = false,
        .config.interrupt = portIntFallingEdge,
    },
    {
        .pinName = GPIO_PINS_OUT_OF_RANGE,
    }
};

/* Declare Output GPIO pins */
gpio_output_pin_user_config_t ledPins[] = {
    {
        .pinName = gpioLED1,
        .config.outputLogic = 1,
        .config.slewRate = portSlowSlewRate,
        .config.driveStrength = portLowDriveStrength,
    },
    {
        .pinName = gpioLED2,
        .config.outputLogic = 1,
        .config.slewRate = portSlowSlewRate,
        .config.driveStrength = portLowDriveStrength,
    },
    {
        .pinName = gpioLED3,
        .config.outputLogic = 1,
        .config.slewRate = portSlowSlewRate,
        .config.driveStrength = portLowDriveStrength,
    },
    {
        .pinName = GPIO_PINS_OUT_OF_RANGE,
    }
};

void Board_Pin_Init(void)
{
	/* Enable Clock gates */
	
	/* Configure pins for and Initialize the RGB LEDs */
	LED1_SELECT;
	LED2_SELECT;
	LED3_SELECT;
	LED1_EN;
	LED2_EN;
	LED3_EN;
	LED1_OFF;
	LED2_OFF;
	LED3_OFF;
	
	/* Configure pin for and initialize SW1 */
	SW1_SELECT;
	SW1_EN;
	
	/* Configure pins for the UART0 */
	
	/* Configure pins for the TSI */
	PORT_Hal_SetMuxMode(PORTB,16u,portPinDisabled);
  PORT_Hal_SetMuxMode(PORTB,17u,portPinDisabled);
}

/* Initialize clock. 
void BOARD_ClockInit(void)
{
    // Set allowed power mode, allow all. 
    SMC_HAL_SetProtection(SMC, kAllowPowerModeAll);

    // Setup board clock source. 
    // Setup OSC0 if used.
    // Configure OSC0 pin mux.
    PORT_HAL_SetMuxMode(EXTAL0_PORT, EXTAL0_PIN, EXTAL0_PINMUX);
    PORT_HAL_SetMuxMode(XTAL0_PORT, XTAL0_PIN, XTAL0_PINMUX);
    BOARD_InitOsc0();

    // Set system clock configuration. 
#if (CLOCK_INIT_CONFIG == CLOCK_VLPR)
    CLOCK_SetBootConfig(&g_defaultClockConfigVlpr);
#else
    CLOCK_SetBootConfig(&g_defaultClockConfigRun);
#endif
}*/
/*
void dbg_uart_init(void)
{
    configure_lpsci_pins(BOARD_DEBUG_UART_INSTANCE);

    // Select different clock source for LPSCI. 
#if (CLOCK_INIT_CONFIG == CLOCK_VLPR)
    CLOCK_SYS_SetLpsciSrc(BOARD_DEBUG_UART_INSTANCE, kClockLpsciSrcMcgIrClk);
#else
    CLOCK_SYS_SetLpsciSrc(BOARD_DEBUG_UART_INSTANCE, kClockLpsciSrcPllFllSel);
#endif

    DbgConsole_Init(BOARD_DEBUG_UART_INSTANCE, BOARD_DEBUG_UART_BAUD, kDebugConsoleLPSCI);
}
*/

