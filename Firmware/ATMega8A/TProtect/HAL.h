/*
 * HAL.h
 *
 * Created: 26.09.2018 13:10:18
 *  Author: fly8r
 */
#ifndef HAL_H_
#define HAL_H_

#include <avr/io.h>


#define HI(x)							((x)>>8)
#define LO(x)							((x)& 0xFF)
#define STATE_ON						1
#define STATE_OFF						0

//------------------------------ System timer configuration for 1mS interrupt
#define SYSTICK_TIME_MS					1
#define SYSTICK_PRESCALER				64UL
#define SYSTICK_OCR_CONST				((F_CPU*SYSTICK_TIME_MS) / (SYSTICK_PRESCALER*1000))
#define SYSTICK_TIMER_COUNTER           TCNT2
#define SYSTICK_TIMER_OCR               OCR2
#define SYSTICK_TIMER_INIT()            { TCCR2=1<<WGM21|1<<CS22; ASSR=0; SYSTICK_TIMER_OCR=SYSTICK_OCR_CONST; SYSTICK_TIMER_COUNTER=0; }
#define SYSTICK_INTERRUPT_ENABLE()      { TIMSK |= 1<<OCIE2; }
#define SYSTICK_INTERRUPT_DISABLE()     { TIMSK &= ~(1<<OCIE2); }

//------------------------------
#define UBRR_CONST						((F_CPU + BAUDRATE*8) / (BAUDRATE*16) - 1)
#define UART_INIT()						{ UBRRL=LO(UBRR_CONST); UBRRH=HI(UBRR_CONST); UCSRA=0; UCSRB=(1<<RXEN|1<<TXEN); UCSRC=(1<<URSEL|1<<UCSZ0|1<<UCSZ1); }
#define UART_TX_INTERRUPT_ENABLE()		{ UCSRB |= (1<<UDRIE); }
#define UART_TX_INTERRUPT_DISABLE()		{ UCSRB &= ~(1<<UDRIE); }
#define UART_RX_INTERRUPT_ENABLE()		{ UCSRB |= (1<<RXCIE); }
#define UART_RX_INTERRUPT_DISABLE()		{ UCSRB &= ~(1<<RXCIE); }


//------------------------------ Configuration IO for 1-wire
#define OW_DDR                          DDRC
#define OW_PORT                         PORTC
#define OW_PIN							PINC
#define OW_MASK							(1<<2)
#define OW_DQ_MODE_INPUT()				{ OW_DDR &= ~OW_MASK; }
#define OW_DQ_MODE_OUTPUT()				{ OW_DDR |= OW_MASK; }
#define OW_DQ_HIGH()                    { OW_DDR &= ~OW_MASK; }
#define OW_DQ_LOW()                     { OW_DDR |= OW_MASK; }
#define OW_DQ_READ_BIT					(OW_PIN & OW_MASK)              // TRUE - if HIGH, FALSE - if LOW
#define OW_INIT()						{ OW_DQ_MODE_INPUT(); }

//------------------------------ Configuration IO for FAN SPEED COUNTER
#define FAN_COUNTER_DDR					DDRD
#define FAN_COUNTER_PORT				PORTD
#define FAN_COUNTER_PIN					PIND
#define FAN_COUNTER_MASK				(1<<2)
#define FAN_COUNTER_INIT()				{ FAN_COUNTER_PORT |= FAN_COUNTER_MASK; FAN_COUNTER_DDR &= ~FAN_COUNTER_MASK; MCUCR |= (1<<ISC01); }
#define FAN_COUNTER_INTERRUPT_ENABLE()	{ GICR |= (1<<INT0); }
#define FAN_COUNTER_INTERRUPT_DISABLE() { GICR &= ~(1<<INT0); }
#ifndef FAN_COUNTER_FAILS
	#define FAN_COUNTER_FAILS			3
#endif

//------------------------------ Configuration IO for FAN control
#define FAN_DDR							DDRB
#define FAN_PORT						PORTB
#define FAN_MASK						(1<<0)
#define FAN_ON()						{ FAN_PORT |= FAN_MASK; }
#define FAN_OFF()						{ FAN_PORT &= ~FAN_MASK; }
#define FAN_INIT()						{ FAN_DDR |= FAN_MASK; FAN_OFF(); }

//------------------------------ Configuration IO for RELAY control
#define RELAY_DDR						DDRC
#define RELAY_PORT						PORTC
#define RELAY_MASK						(1<<5)
#define RELAY_ON()						{ RELAY_PORT |= RELAY_MASK; }
#define RELAY_OFF()						{ RELAY_PORT &= ~RELAY_MASK; }
#define RELAY_STATE						(RELAY_PORT & RELAY_MASK)
#define RELAY_INIT()					{ RELAY_DDR |= RELAY_MASK; RELAY_OFF(); }

//------------------------------ Configuration IO for BUTTON
#define BTN_DDR							DDRD
#define BTN_PORT						PORTD
#define BTN_PIN							PIND
#define BTN_MASK						(1<<5)
#define BTN_INIT()						{ BTN_PORT |= BTN_MASK; BTN_DDR &= ~BTN_MASK; }
#define BTN_PRESSED						((BTN_PIN & BTN_MASK) == 0)

//------------------------------ Configuration IO for GREEN LED
#define LED_GRN_DDR						DDRD
#define LED_GRN_PORT					PORTD
#define LED_GRN_MASK					(1<<6)
#define LED_GRN_ON()					{ LED_GRN_PORT |= LED_GRN_MASK; }
#define LED_GRN_OFF()					{ LED_GRN_PORT &= ~LED_GRN_MASK; }
#define LED_GRN_TOGGLE()				{ LED_GRN_PORT ^= LED_GRN_MASK; }
#define LED_GRN_INIT()					{ LED_GRN_DDR |= LED_GRN_MASK; LED_GRN_OFF(); }

//------------------------------ Configuration IO for PROTECTSTATE LED
#define LED_RED_DDR						DDRD
#define LED_RED_PORT					PORTD
#define LED_RED_MASK					(1<<7)
#define LED_RED_ON()					{ LED_RED_PORT |= LED_RED_MASK; }
#define LED_RED_OFF()					{ LED_RED_PORT &= ~LED_RED_MASK; }
#define LED_RED_TOGGLE()				{ LED_RED_PORT ^= LED_RED_MASK; }
#define LED_RED_INIT()					{ LED_RED_DDR |= LED_RED_MASK; }


#endif


