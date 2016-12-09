#include "includes.h" 

static bool system_alarm = 0;

smc_power_mode_config_t powerMode = 
{
	.powerModeName = powerModeWait
};

msg_queue_t msgQueue;
msg_queue_handler_t msgQueue_Handler;
uint8_t msg;

uint32_t event = 0;

lptmr_state_t lptmrState;
volatile uint16_t pitCounter = 0;

tsi_state_t tsiState;
uint8_t tsi_Channel[BOARD_TSI_ELECTRODE_CNT];
uint32_t unTouch;

static void ST_TaskInit(void);
static void ST_processAppMsg(uint8_t *pMsg);
static void ST_processReadTempEvt(void);
static void ST_processSenseTouchEvt(void);

void PIT_User_Callback(void)
{
	++pitCounter;
	
	event |= ST_SENSE_TOUCH_EVT;
	
	if(pitCounter == 500)
	{
		event |= ST_READ_TEMP_EVT;
	}
	if(pitCounter == 1000)
	{
		event |= ST_READ_TEMP_EVT;
 		event |= ST_BLINK_LED_EVT;
		pitCounter = 0;
	}
}

void PORTD_IRQHandler(void)
{
	uint8_t msg = ST_KEY_PRESSED_MSG;
	SYS_MsgEnqueue(msgQueue_Handler, &msg);
}

static void ST_TaskInit(void)
{
	uint8_t i;
	
	static const lptmr_user_config_t lptmrUserConfig = 
	{
		.timerMode= lptmrTimerModeTimeCounter,
		.freeRunningEnable = true,
		.prescalerEnable = false,
		.prescalerClockSource = clockLptmrSrcLpoClk,
		.isInterruptEnabled = false,
	};
	
	static const tsi_config_t tsiHwConfig =
	{
		.ps = TSIElecOscPrescaler_2div,
		.exchargecurrent = TSIExtOscChargeCurrent_8uA,
		.refchargecurrent = TSIRefOscChargeCurrent_8uA,
		.nscan = TSI_Scan_8time,
		.mode = TSIAnalogModeSel_Capacitive,
		.dvolt = TSIOscVolRails_Dv_103,
		.thresh = TSI_HIGHTHRESHOLD,
		.thresl = TSI_LOWTHRESHOLD,
	};
	
	const tsi_user_config_t tsiUserConfig = 
	{
		.config = (tsi_config_t *)&tsiHwConfig,
		.callback = NULL,
		.usrData = 0
	};
	
	static const pit_user_config_t ch0Config = 
	{
		.isInterruptEnabled = true,
			.periodUs = ST_PERIODIC_EVT_UNITPERIOD,
		.callbackFunc = PIT_User_Callback
	};
	
	/* LPTMR Initialization */
	
	LPTMR_Init(&lptmrState, &lptmrUserConfig);	// Initialze LPTMR and store LPTMR state	
	LPTMR_Start();	//Start the LPTMR
	
	/* PIT Initialization */
	PIT_Init(true);
	PIT_InitChannel(0, &ch0Config);
	
	/* Application-Specific ADC Initialization */
	app_ADCInit();
	
	/* TSI Initialization */
	tsi_Channel[0] = BOARD_TSI_ELECTRODE_1;
	tsi_Channel[1] = BOARD_TSI_ELECTRODE_2;
	TSI_Init(&tsiState, &tsiUserConfig);
	for(i = 0; i < BOARD_TSI_ELECTRODE_CNT; i++)
	{
		TSI_EnableElectrode(tsi_Channel[i], true);
	}
	unTouch = TSI_GetUnTouchBaseline(tsi_Channel);
	
	/* UART0 for logging Initialization */
	uart0_Init(9600,0,0,8,1);
	
	/* Construct a message queue */
	msgQueue_Handler = SYS_MsgQueueCreate(&msgQueue, ST_MSG_QUEUE_SIZE);
	
	/* System Interrupt setting */
	// Configure interrupts' priorities 
	SYS_EnableIRQGlobal(); // Enable system interrupt
	
	PIT_StartTimer(0);
	
	log_Raw(SYSTEM_INITIALIZED);
}

void ST_TaskFunc(void)
{
	
	ST_TaskInit();	// Task Initialization 
	for(;;)
	{
		// Entering low power mode -- semaphore pending state
		//smc_Hal_SetPowerMode(&powerMode);
		//log_Raw(SYSTEM_ENTERED_WAIT);
	/* Process app message */
		while (status_SYS_Success == SYS_MsgDequeue(msgQueue_Handler, &msg))
		{
			if (msg)
			{
				ST_processAppMsg(&msg);
			}
		}
		
	/* Perform periodic tasks */
		if(event & ST_SENSE_TOUCH_EVT)
		{
			event &= ~ST_SENSE_TOUCH_EVT;
			ST_processSenseTouchEvt();
		}
		
		if(event & ST_READ_TEMP_EVT)
		{
			event &= ~ST_READ_TEMP_EVT;// clear event bits
			ST_processReadTempEvt();			
		}
	
		if(event & ST_BLINK_LED_EVT)
		{
			event &= ~ST_BLINK_LED_EVT;
			if(!system_alarm)
			{
				//BLINK LED
				LED2_OFF;
				LED1_TOGGLE;
				log_Raw(SYSTEM_NORMAL);
			}
			else
			{
				LED1_OFF;
				LED2_ON;
				
			}
		}
	}
}

static void ST_processAppMsg(uint8_t *pMsg)
{
	switch (*pMsg)
	{
		case ST_KEY_PRESSED_MSG:
			LED1_ON;
			LED2_ON;
			LED3_ON;
			SYS_TimeDelay(10);
			LED1_OFF;
			LED2_OFF;
			LED3_OFF;
			break;
			
		default:	//do nothing
			break;
	}
}
static void ST_processReadTempEvt(void)
{
	int32_t temp;
	
	log_Raw(READING_SYS_TEMPERATURE);
	temp = read_OnChipTemperature(); 
	log_Raw((uint8_t)temp);
	/* check if temperature is abnormal */
	if((temp > 27) || (temp < 20))
	{
		system_alarm = true;
		log_Raw(SYSTEM_TEMPERATURE_NORMAL);
	}
	else
	{
		system_alarm = false;
		log_Raw(SYSTEM_TEMPERATURE_OUTOFRANGE);
	}
}

static void ST_processSenseTouchEvt(void)
{
	uint8_t i;
	uint16_t measureResult[BOARD_TSI_ELECTRODE_CNT];
	uint32_t avgMeasure;
	
	if(TSI_MeasureBlocking() != status_TSI_Success)
	{
		return;
	}

	// Init average measurement.
  avgMeasure = 0;
    for(i = 0; i < BOARD_TSI_ELECTRODE_CNT; i++)
    {
			if(TSI_GetCounter(tsi_Channel[i], &measureResult[i]) != status_TSI_Success)
			{
				return;
      }
      avgMeasure += measureResult[i];
    }
    // Calculates average measurement.
    avgMeasure /= BOARD_TSI_ELECTRODE_CNT;

    // Check if it's the touched state.
	// Add 10 to remove noise
    if(avgMeasure > unTouch + 10)
    {
        LED3_ON;
				log_Raw(SYSTEM_SENSED_TOUCH);
    }
    else
    {
        LED3_OFF;
    }
}
