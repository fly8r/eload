/*
 * ELoad.c
 *
 * Created: 26.09.2018 11:29:43
 * Author : fly8r
 */
#include "main.h"


/************************************************************************/
/* Code                                                                 */
/************************************************************************/

/* Functions prototypes */
void InitGPIO(void);


int main(void)
{
	// Main initialize
	InitGPIO();
	InitMessages();
	InitTimers();
	// FSMs initialize
	FSM_SYSTEM_Init();
	FSM_UART_Init();
	FSM_OWS_Init();
	FSM_DS18X20_Init();
	FSM_FAN_Init();
	FSM_BUTTON_Init();
	FSM_LEDS_Init();

	// Enable global interrupts
	sei();



    /* Main cycles */
    while (1)
    {
		//
		ProcessTimers(&systick);

		FSM_SYSTEM_Process();
		FSM_UART_Process();
		FSM_OWS_Process();
		FSM_DS18X20_Process();
		FSM_FAN_Process();
		FSM_BUTTON_Process();
		FSM_LEDS_Process();



		//
		ProcessMessages();
    }
}


//------------------------------ MCU IO initialization
inline void InitGPIO(void)
{
	// Initialize systick timer
	SYSTICK_TIMER_INIT();
	SYSTICK_INTERRUPT_ENABLE();
	// Initialize IO for 1-wire
	OW_INIT();
	// Initialize IO for Button input
	BTN_INIT();
	// Initialize IO for Relay control
	RELAY_INIT();
	// Initialize IO for FAN control
	FAN_INIT();
	// Initialize IO for FAN counter input
	FAN_COUNTER_INIT();
	FAN_COUNTER_INTERRUPT_ENABLE();
	// Initialize IO for GREEN LED control
	LED_GRN_INIT();
	// Initialize IO for RED LED control
	LED_RED_INIT();
	// Initialize UART
	UART_INIT();
	UART_TX_INTERRUPT_ENABLE();
	// Initialize IO for 1-wire
	OW_INIT();
}

//------------------------------ System tick interrupt
ISR(TIMER2_COMP_vect)
{
	systick++;
}

//------------------------------ FAN counter interrupt on INT0
ISR(INT0_vect)
{
	fan.counter++;
}

//------------------------------ USART RX Interrupt vector
ISR(USART_RXC_vect)
{
	UART_RX_Service();
}

//------------------------------ USART UDRE Interrupt vector
ISR(USART_UDRE_vect)
{
	UART_TX_Service();
}