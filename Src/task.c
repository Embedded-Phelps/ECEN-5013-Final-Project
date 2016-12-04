msg_queue_t msgQueue;
msg_queue_handler_t msgQueue_Handler;

lptmr_state_t lptmrState;
//(do it in main)
//Power setting
//System Clock setting
//GPIO Configuration 
//LPTMR Initialization
//ADC Initialization
//TSI Initialization
//UART Initialzaition
//Callbacks Registering
//Interrupt Setting


void ST_Init(void)
{
	lptmr_user_config_t lptmrUserConfig = 
	{
		.timerMode= kLptmrTimerModeTimeCounter,
		.freeRunningEnable = false;
		.prescalerEnable = false;
		.prescalerClockSource = kClockLptmrSrcLpoClk,
		.isInterruptEnabled = true;
	}
	
	/* LPTMR Initialization */
	
	LPTMR_Init(&lptmrState, &lptmrUserConfig);	// Initialze LPTMR and store LPTMR state
	LPTMR_SetTimerPeriodUs(1);	// Set up LPTMR unit period
	LPTMR_InstallCallback(lptmr_isr_callback);	// Register LPTMR callback function 
	
	/* Application-Specific ADC Initialization */
	app_ADCInit();
	
	/* TSI Initialization */
	
	/* UART0 for logging Initialization */
	
	/* Construct a message queue */
	msgQueue_Handler = SYS_MsgQueueCreate(msgQueue, 16);
	
	/* System Interrupt setting */
	// Configure interrupts' priorities 
	// Enable system interrupt
	
	LPTMR_Start();	//Start the LPTMR
}

void ST_TaskFunc()
{
	ST_Init();	// Task Initialization 
	
	for(;;)
	{
		// Entering low power mode -- semaphore pending state
		
	/* Process app message */
		while (status_SYS_ Sucess == SYS_MsgDequeue(msgQueue_Handler, msg))
		{
			if (*msg)
			{
				ST_processAppMsg(msg);
			}
		}
		
	/* Perform periodic tasks */
		if(event bit is set)
		{
			// clear event bits
			// start the timer again
			// perform periodic task
			// blink the led if everything is operating normally
		}
	}
}

static void ST_processAppMsg(evt_t *pMsg)
{
	switch (pMsg->event)
	{
		case:
			break;
			
		case:
			break;
			
		default:	//do nothing
			break;
	}
}
static void ST_performPeriodicTask(void)
{
	
}

