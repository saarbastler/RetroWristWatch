/*
 * RetroUhr.cpp
 *
 * Created: 31.07.2016 19:25:52
 * Author : Joerg
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "Device.h"
#include "Segment.h"
#include "Timer1.h"
#include "DelayTimerW.h"
#include "I2CMaster.h"
#include "DS1337.h"
#include "RetroUhr.h"

#ifdef DEBUG

#include "PrintStream.h"
#include "UartNS.h"
#include "AtmegaMonitor.h"

extern DS1337 ds1337;
extern UartNS uartNS;
PrintStream out( &uartNS );

char monitorBuffer[40];
class Monitor : public AtmegaMonitor
{
  public:
  Monitor(): AtmegaMonitor( monitorBuffer, 40, &out) { }
  protected:

  // extension, return 1, if command is executed, 0 on error
  virtual BYTE executeCommand( char ch );
  
  BYTE setTime();
};
Monitor monitor;
UartNS uartNS( UART0, 57600, &monitor );

DelayTimerWord led( Timer1::getTickerAddr(), 500 );
#endif

void debugIdle()
{
#ifdef DEBUG
  uartNS.cyclic();
    
  /*if( led.isDone() )
  {
    led.start();
      
    LED1_TOGGLE();
  }*/
#endif
}

int main(void)
{
#ifdef DEBUG 
  LED1_CONFIGURE();
  LED1_ON();
  
  led.start();  
  
  out.print_P( PSTR("\r\nRetro Uhr\r\n") );
#endif

  RetroUhr::run();
}

#ifdef DEBUG
BYTE Monitor::executeCommand( char ch )
{
  WORD idx, data;
  
  switch( ch )
  {
    case 'b':
    idx= nextHexWord();
    if( !evaluateError )
    {
      data=nextHexWord();
      if( !evaluateError )
      {
        Segment::setDisplay( idx, data );
        return 1;
      }      
    }
    return 0;
    
    case 'd':
    idx= nextHexWord();
    if( !evaluateError )
    {
      data=nextHexWord();
      if( !evaluateError )
      {
        Segment::setDisplayDigit( idx, data );
        return 1;
      }
    }
    return 0;

    case 'p':
    idx= nextHexWord();
    if( !evaluateError )
    {
      data=nextHexWord();
      if( !evaluateError )
      {
        Segment::setDP( idx, data);
        return 1;
      }
    }
    return 0;

    case 'v':
    idx= nextHexWord();
    if( !evaluateError )
    {
      Segment::setHex(idx);
      return 1;
    }
    return 0;
    
    case '0':
      Segment::enableDisplay(0);
      return 1;
    case '1':
      Segment::enableDisplay(1);
      return 1;
      
    case 'i':
      //ds1337.readRegister(0, 16, buffer);
      ds1337.readDateAndTime();
      
      if( I2CMaster::hasError())
      {
        out.printf_P(PSTR("DS1337 Error: %bx\r\n"), I2CMaster::getError());
      }        
      else
      {
        //out.dumpMemory(PSTR("DS1337: "), buffer, 16);
        out.printHex( ds1337.getHours());
        out.putch(':');
        out.printHex( ds1337.getMinutes());
        out.putch(':');
        out.printHex( ds1337.getSeconds());
        out.putch(' ');
        out.printHex( ds1337.getDate());
        out.putch('.');
        out.printHex( ds1337.getMonth());
        out.putch('.');
        out.printHex( ds1337.getYear());
        out.putch('\r');
        out.putch('\n');        
      }        
        
      return 1;
      
    case 's':
      return setTime();
  }
  
  return AtmegaMonitor::executeCommand(ch);
}


BYTE Monitor::setTime()
{
  WORD w= nextHexWord();
  if( evaluateError )
    return 0;
  if( w & 0xff00 )
    return 0;
  ds1337.set24H(1);
  ds1337.setHours(w);
  
  char ch= nextCharIgnoreBlank();
  if( ch != ':')
    return 0;
    
  w= nextHexWord();
  if( evaluateError )
    return 0;
  if( w & 0xff00 )
    return 0;
  ds1337.setMinutes(w);
  
  ch= nextCharIgnoreBlank();
  if( ch != ':')
    return 0;
  
  w= nextHexWord();
  if( evaluateError )
    return 0;
  if( w & 0xff00 )
    return 0;
  ds1337.setSeconds(w);
    
  ch= nextChar();
  if( ch != ' ')
    return 0;
  
  w= nextHexWord();
  if( evaluateError )
    return 0;
  if( w & 0xff00 )
    return 0;
  ds1337.setDate(w);
  
  ch= nextCharIgnoreBlank();
  if( ch != '.')
    return 0;
  
  w= nextHexWord();
  if( evaluateError )
    return 0;
  if( w & 0xff00 )
    return 0;
  ds1337.setMonth(w);
  
  ch= nextCharIgnoreBlank();
  if( ch != '.')
  return 0;
  
  w= nextHexWord();
  if( evaluateError )
    return 0;
  if( w > 0x2199 || w < 0x2000 )
    return 0;
  ds1337.setYear(w);
  
  ds1337.writeDateAndTime();
  
  if( I2CMaster::hasError())
  {
    out.printf_P(PSTR("DS1337 Error: %bx\r\n"), I2CMaster::getError());
  }
  
 return 1;
}
#endif