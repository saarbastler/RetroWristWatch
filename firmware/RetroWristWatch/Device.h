/*
 * Device.h
 *
 * Created: 30.11.2013 21:22:14
 *  Author: Jörg
 */ 


#ifndef DEVICE_H_
#define DEVICE_H_

#include "integer.h"

#ifdef DEBUG
#define LED1                5
#define LED1_PORT           PORTB
#define LED1_DDR            DDRB
#define LED1_PIN            PINB
#define LED1_CONFIGURE()    LED1_DDR |= _BV(LED1)
#define LED1_ON()           LED1_PORT |= _BV(LED1)
#define LED1_OFF()          LED1_PORT &= ~_BV(LED1)
#define LED1_TOGGLE()       PINB |= _BV(LED1)
#endif

#define DEBOUNCE_TIME     5
#define BUT_LONG_TIME     500

#define I2C_SYNCHONOUS
#define I2C_INTERRUPT

#ifdef DEBUG
#define F_CPU 16000000
//#define F_CPU 8000000
#else
#define F_CPU 1000000
#endif
#define ATMEGA_MONITOR_PORTS    'P'
#define ATMEGA_MONITOR_CPU      'C'
#define ATMEGA_MONITOR_MEMORY   'M'
#define ATMEGA_MONITOR_TIMER    'T'
#define ATMEGA_MONITOR_EEPROM   'E'

#define BUTTONL_PORT    PORTD
#define BUTTONL_DDR     DDRD
#define BUTTONL_PIN     PIND
#define BUTTONL         2
#define BUTTONR_PORT    PORTD
#define BUTTONR_DDR     DDRD
#define BUTTONR_PIN     PIND
#define BUTTONR         3

// Digit1 PC1
// Digit2 PC0
// Digit3 PB6 (PB3 on Arduino Nano)
// Digit4 PD5
// a      PD6
// b      PD7
// c      PD4
// d      PB1
// e      PC3
// f      PB2
// g      PB0
// dp     PB7 (PB4 on Arduino Nano)

#define DIGIT1          1
#define DIGIT1_PORT     PORTC
#define DIGIT2          0
#define DIGIT2_PORT     PORTC

#ifdef DEBUG
#define DIGIT3          3
#else
#define DIGIT3          6
#endif

#define DIGIT3_PORT     PORTB
#define DIGIT4          5
#define DIGIT4_PORT     PORTD

#define SEGMENT_A       6
#define SEGMENT_A_PORT  PORTD
#define SEGMENT_B       7
#define SEGMENT_B_PORT  PORTD
#define SEGMENT_C       4
#define SEGMENT_C_PORT  PORTD
#define SEGMENT_D       1
#define SEGMENT_D_PORT  PORTB
#define SEGMENT_E       3
#define SEGMENT_E_PORT  PORTC
#define SEGMENT_F       2
#define SEGMENT_F_PORT  PORTB
#define SEGMENT_G       0
#define SEGMENT_G_PORT  PORTB

#ifdef DEBUG
#define SEGMENT_DP      4
#else
#define SEGMENT_DP      7
#endif

#define SEGMENT_DP_PORT PORTB

#define PORTB_BITS    (_BV(SEGMENT_DP)|_BV(DIGIT3)|_BV(SEGMENT_F)|_BV(SEGMENT_D)|_BV(SEGMENT_G))
#define PORTC_BITS    (_BV(SEGMENT_E)|_BV(DIGIT1)|_BV(DIGIT2))
#define PORTD_BITS    (_BV(SEGMENT_B)|_BV(SEGMENT_A)|_BV(DIGIT4)|_BV(SEGMENT_C))

#endif /* DEVICE_H_ */