/*
 * UartNS.h Uart Non Static
 *
 * Created: 14.06.2015 18:19:01
 *  Author: Jörg
 */ 


#ifndef UARTNS_H_
#define UARTNS_H_

#include "integer.h"
#include "ByteReceiver.h"
#include "UartDefines.h"

class UartNS : public ByteReceiver
{
public:

  // uart is one of UARTx constants
  UartNS( volatile BYTE *uart, DWORD baudrate, ByteReceiver *byteReceiver );

	void cyclic();

	BYTE transmitComplete();
	BYTE readyToSend();
  
  virtual void received( BYTE data )    { putch( data ); }
	void putch( BYTE ch );

private:

  ByteReceiver *byteReceiver;
  volatile BYTE *ucsrXa;
  
};



#endif /* UARTNS_H_ */