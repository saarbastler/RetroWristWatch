/*
 * UartNS.cpp
 *
 * Created: 14.06.2015 18:26:19
 *  Author: Jörg
 */ 

#include <avr/io.h>

#include "device.h"
#include "UartNS.h"

#define _UCSRA       ucsrXa[0]
#define _UCSRB       ucsrXa[1]
#define _UCSRC       ucsrXa[2]
#define _UBBRL       ucsrXa[4]
#define _UBBRH       ucsrXa[5]
#define _UDR         ucsrXa[6]

UartNS::UartNS( volatile BYTE *uart, DWORD baudrate, ByteReceiver *_byteReceiver ) 
  : byteReceiver( _byteReceiver ), ucsrXa(uart)
{
  if( 
#ifdef UART0
      uart == UART0 
#endif
#ifdef UART1
#ifdef UART0
    ||
#endif
      uart == UART1
#endif
#ifdef UART2
      || uart == UART2 
#endif
#ifdef UART3
      || uart == UART3
#endif
      )
  {
    WORD ubbr= (WORD)(.5 + F_CPU / (baudrate * 16))-1;
    
    _UBBRH= ubbr >> 8;
    _UBBRL= ubbr;
    _UCSRA= 0;
    _UCSRB= _BV(_RXEN)|_BV(_TXEN);

#ifdef _URSEL
    _UCSRC= _BV(_URSEL) | (3 << _UCSZ);
#else
    _UCSRC= 3 << _UCSZ;    
#endif
  }
}

void UartNS::cyclic()
{
	if( _UCSRA & _BV( _RXC ) )
	{
  	BYTE data= _UDR;

  	if( byteReceiver != 0 )
  	{
    	byteReceiver->received( data );
  	}
	}
}

BYTE UartNS::transmitComplete()
{
  return (_UCSRA & (1 << _TXC));
}

BYTE UartNS::readyToSend()
{
  return (_UCSRA & (1 << _UDRE));  
}

void UartNS::putch( BYTE ch )
{
  while(! readyToSend())
  ;

  _UDR= ch;
}
