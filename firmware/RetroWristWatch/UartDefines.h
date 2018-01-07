/*
 * UartDefines.h
 *
 * Created: 23.06.2015 10:36:27
 *  Author: Jörg
 */ 


#ifndef UARTDEFINES_H_
#define UARTDEFINES_H_

// valid values for UartNS constructor
#ifdef UCSRA
#define UART0       &UCSRA

#define _RXCIE      RXCIE
#define _RXEN       RXEN
#define _TXEN       TXEN
#define _UDRIE      UDRIE
#define _RXC        RXC
#define _TXC        TXC
#define _RXEN       RXEN
#define _TXEN       TXEN
#define _UCSZ       UCSZ0
#define _UDRE       UDRE
#define _URSEL      URSEL
#endif

#ifdef UCSR0A
#define UART0       &UCSR0A

#define _RXCIE      RXCIE0
#define _RXEN       RXEN0
#define _TXEN       TXEN0
#define _UDRIE      UDRIE0
#define _RXC        RXC0
#define _TXC        TXC0
#define _RXEN       RXEN0
#define _TXEN       TXEN0
#define _UCSZ       UCSZ00
#define _UDRE       UDRE0
#endif

#ifdef UCSR1A
#define   UART1       &UCSR1A
/*
#define _RXCIE      RXCIE1
#define _RXEN       RXEN1
#define _TXEN       TXEN1
#define _UDRIE      UDRIE1

#ifndef _RXC
#define _RXC        RXC1
#define _TXC        TXC1
#define _RXEN       RXEN1
#define _TXEN       TXEN1
#define _UCSZ       UCSZ10
#define _UDRE       UDRE1
#endif
*/
#endif

#ifdef UCSR2A
#define   UART2       &UCSR2A
#endif

#ifdef UCSR3A
#define   UART3       &UCSR3A
#endif




#endif /* UARTDEFINES_H_ */