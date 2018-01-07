/* 
* RetroUhr.cpp
*
* Created: 06.08.2016 10:46:21
* Author: Joerg
*/


#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>

#include "Device.h"
#include "Segment.h"
#include "Timer1.h"
#include "DelayTimerW.h"
#include "I2CMaster.h"
#include "DS1337.h"
#include "RetroUhr.h"
#include "ButtonDebounce.h"

#ifdef DEBUG
#include <avr/pgmspace.h>
#include "PrintStream.h"
extern PrintStream out;
extern void debugIdle();
#endif

void idle();
void powerSave();
void buttonCallback(BYTE id, BYTE type);

#define ID_LEFT   0
#define ID_RIGHT  1

ButtonDebounce buttonL( &BUTTONL_PIN, _BV(BUTTONL), Timer1::getTickerAddr(), ID_LEFT, buttonCallback);
ButtonDebounce buttonR( &BUTTONL_PIN, _BV(BUTTONR), Timer1::getTickerAddr(), ID_RIGHT, buttonCallback);

DS1337 ds1337;
BYTE mode;
BYTE oldSeconds;
WORD timeout;
WORD powersaveTimeout;
BYTE flags;

#define FLAG_SHOW_DOT         7
#define FLAG_FLASH            6
#define FLAG_INDEX            1
#define FLAG_INDEX_MASK       0x03

#define MODE_POWERSAVE        0
#define MODE_SHOW_TIME        1
#define MODE_SHOW_TIME_SEC    2
#define MODE_SHOW_TIME_DATE   3
#define MODE_SHOW_TIME_YEAR   4
#define MODE_SET_TIME         5
#define MODE_SET_DATE         6
#define MODE_SET_YEAR         7
#define MODE_ERROR            8
#define MODE_INIT             9

const BYTE message[] PROGMEM= { 0x1f, 0x3f, 0x79, 0x77, 0x7d, 0x00, 0x73, 0x77, 0x06, 0x37 };
signed char midx;

void showTime(BYTE newMode)
{
  BYTE b;
  switch( newMode )
  {
    case MODE_SHOW_TIME:
    case MODE_SET_TIME:
      b= ds1337.getHours();
      Segment::setDisplayDigit( 0, b >> 4);
      Segment::setDisplayDigit( 1, b );
      if( flags & _BV(FLAG_SHOW_DOT))
        Segment::setDP(1,1);
    
      b= ds1337.getMinutes();
      Segment::setDisplayDigit( 2, b >> 4);
      Segment::setDisplayDigit( 3, b );

      if( mode == MODE_SET_TIME && flags & _BV(FLAG_FLASH))
      {
        Segment::setDisplay( flags & FLAG_INDEX_MASK, 0);
      }
      
      if(oldSeconds != ds1337.getSeconds())
      {
        flags ^= _BV(FLAG_SHOW_DOT);
      }
      break;
      
    case MODE_SHOW_TIME_SEC:
      Segment::setDisplay(0, 0);
      Segment::setDisplay(1, 0);
      b= ds1337.getSeconds();
      Segment::setDisplayDigit( 2, b >> 4);
      Segment::setDisplayDigit( 3, b );
      break;
            
    case MODE_SHOW_TIME_DATE:
    case MODE_SET_DATE:
      b= ds1337.getDate();
      Segment::setDisplayDigit( 0, b >> 4);
      Segment::setDisplayDigit( 1, b );
      Segment::setDP(1,1);

      b= ds1337.getMonth();
      Segment::setDisplayDigit( 2, b >> 4);
      Segment::setDisplayDigit( 3, b );    
      Segment::setDP(3,1);
      if( mode == MODE_SET_DATE && flags & _BV(FLAG_FLASH))
      {
        if(flags & _BV(FLAG_INDEX))
        {
          Segment::setDisplay( 2, 0);
          Segment::setDisplay( 3, 0);          
        }
        else
        {
          Segment::setDisplay( 0, 0);
          Segment::setDisplay( 1, 0);
        }
      }
      break;
      
    case MODE_SHOW_TIME_YEAR:
    case MODE_SET_YEAR:
      {
        WORD y= ds1337.getYear();
        Segment::setHex(y);
      }
      if( mode == MODE_SET_YEAR && flags & _BV(FLAG_FLASH))
      {
        if(flags & _BV(FLAG_INDEX))
        {
          Segment::setDisplay( 2, 0);
          Segment::setDisplay( 3, 0);
        }
        else
        {
          Segment::setDisplay( 0, 0);
          Segment::setDisplay( 1, 0);
        }
      }
      break;
  }      
}

void setMode( BYTE newMode )
{
  switch(newMode)
  {
    case MODE_INIT:
      Segment::enableDisplay(1);
      midx= -3;
      break;
      
    case MODE_POWERSAVE:
      Segment::enableDisplay(0);
      // CPU Idle
      powerSave();
      break;
      
    case MODE_SHOW_TIME:
    case MODE_SHOW_TIME_SEC:
    case MODE_SHOW_TIME_DATE:
    case MODE_SHOW_TIME_YEAR:
    case MODE_SET_TIME:
      flags |= _BV(FLAG_FLASH);
      showTime( newMode );
      Segment::enableDisplay(1);
      timeout= Timer1::getTickerWord();
      break;
      
    case MODE_ERROR:
      Segment::enableDisplay(1);
      Segment::setHex(0xeeee);
      break;
  }
  
  if( mode != newMode )
  {
#ifdef DEBUG   
    out.printf_P( PSTR("M %bx -> %bx\r\n"), mode, newMode);
#endif
    mode= newMode;
  }
}

void readTimeAndShow( BYTE nextMode )
{
  if( I2CMaster::isBusy() )
  {
    setMode(MODE_ERROR);
  }
  else
  {
    oldSeconds= ds1337.getSeconds();
    ds1337.readDateAndTime();
    if( mode == MODE_POWERSAVE )
    {
      oldSeconds= ds1337.getSeconds();
      flags &= ~_BV(FLAG_SHOW_DOT);
    }
  
    if(I2CMaster::hasError())
    {
      setMode( MODE_ERROR );
    }
    else
    {
      setMode( nextMode );
    }  
  }  
}

void incTime(BYTE idx)
{
  BYTE b;
  if( idx < 2 )
  {
    b= ds1337.getHours();
    if( idx == 0 )
    {
      if( b >= 0x23 )
        b= 0;
      else if( b >= 0x20 )
        b &= 0x0f;
      else
        b += 0x10;
    }
    else
    {
      if( (b >= 0x22) || (b & 0x0f) >= 9 )
        b &= 0xf0;
      else
        b++;
    }
    ds1337.setHours(b);
  }
  else
  {
    b= ds1337.getMinutes();
    if( idx == 2 )
    {
      if( b >= 0x50 )
        b &= 0x0f;
      else
        b += 0x10;
    }
    else
    {
      if( (b & 0x0f) >= 9 )
        b &= 0xf0;
      else
        b++;
    }
    ds1337.setMinutes(b);    
  }
}

BYTE isLeapYear()
{
  WORD w= ds1337.getYear();
  
  // dividable by 1000
  if( (w & 0xfff) == 0 )
    return 1;
    
  // dividable by 4 ?
  BYTE b= 10 * ((w & 0xf0) >> 4) + (w & 0xf);
  if(b & 3)
    return 0;
  
  return (w & 0xff) != 0;
}

BYTE getDaysOfMonth()
{
  BYTE m= ds1337.getMonth();
  
  if( m == 2 )
    return isLeapYear() ? 0x29 : 0x28;
    
  if( m < 8 )
    return 0x30 + (m & 1);
  else
    return 0x30 + ((m+1) & 1);
}

void incDate(BYTE idx)
{
  BYTE d= ds1337.getDate();
  BYTE m= ds1337.getMonth();
  BYTE days= getDaysOfMonth();
  
  if( idx )
  {
    if( m == 0x09 )
      m= 0x10;
    else if( m >= 0x12 )
      m= 1;
    else
      m++;
    ds1337.setMonth(m);
    
    if( d >= days )
      ds1337.setDate(days);
  }
  else
  {
    if( d >= days )
      d= 1;
    else if( (d & 0x0f) >= 9 )
      d += 0x7;
    else
      d++;
    ds1337.setDate(d);
  }
}

void incYear(BYTE idx)
{
  WORD w = ds1337.getYear();
  if( idx == 0 )
  {
    if( (w & 0xff00) == 0x2000 )
      w += 0x100;
    else
      w -= 0x100;
  }
  else
  {
    if( (w & 0xf) >= 9 )
      w += 7;
    else if( (w & 0xff) == 0x99 )
      w &= 0xff00;
    else
      w++;
  }
  ds1337.setYear(w);
}

void buttonCallback(BYTE id, BYTE type)
{
#ifdef DEBUG
  out.printf_P( PSTR("BUT %bx: %bx\r\n"), id, type);
#endif
  powersaveTimeout= Timer1::getTickerWord();
  switch( mode )
  {
    case MODE_POWERSAVE:
      readTimeAndShow( id == ID_RIGHT ? MODE_SHOW_TIME : MODE_SHOW_TIME_DATE);
      break;
      
    case MODE_SHOW_TIME:
      if( id == ID_RIGHT && type == BUT_DOWN)
        setMode(MODE_SHOW_TIME_SEC);
      else if( id == ID_LEFT && type == BUT_LONG)
      {
        flags &= ~FLAG_INDEX_MASK;
        setMode(MODE_SET_TIME);
      }      
      break;
      
    case MODE_SHOW_TIME_SEC:
      if( id == ID_RIGHT && type == BUT_DOWN)
        setMode(MODE_SHOW_TIME);
      break;
      
    case MODE_SHOW_TIME_DATE:
      if( id == ID_LEFT && type == BUT_DOWN)
        setMode(MODE_SHOW_TIME_YEAR);
      else if( id == ID_RIGHT && type == BUT_LONG)
        setMode(MODE_SET_DATE);
      break;
    
    case MODE_SHOW_TIME_YEAR:
      if( id == ID_LEFT && type == BUT_DOWN)
        setMode(MODE_SHOW_TIME_DATE);
      else if( id == ID_RIGHT && type == BUT_LONG)
        setMode(MODE_SET_YEAR);
      break;
    
    case MODE_SET_TIME:
      if( type == BUT_DOWN )
      {
        if( id == ID_LEFT )
        {
          flags= (flags & ~FLAG_INDEX_MASK) | ((flags+1) & FLAG_INDEX_MASK) | _BV(FLAG_FLASH);
        }
        else if( id == ID_RIGHT )
        {
          incTime( flags & FLAG_INDEX_MASK );
          showTime( mode );
        }
      }
      else if( type == BUT_LONG )
      {
        if( id == ID_LEFT )
        {
          if(I2CMaster::isBusy())
          {
            setMode(MODE_ERROR);
            return;
          }
          ds1337.setSeconds(0);
          ds1337.writeDateAndTime();
        }
        readTimeAndShow( MODE_SHOW_TIME );        
      }
      break;
      
    case MODE_SET_DATE:
      if( type == BUT_DOWN )
      {
        if( id == ID_LEFT )
        {
          flags= (flags ^ _BV(FLAG_INDEX)) | _BV(FLAG_FLASH);
        }
        else if( id == ID_RIGHT )
        {
          incDate( flags & _BV(FLAG_INDEX) );
          showTime( mode );
        }
      }
      else if( type == BUT_LONG )
      {
        if( id == ID_LEFT )
        {
          if(I2CMaster::isBusy())
          {
            setMode(MODE_ERROR);
            return;
          }
          ds1337.writeDate();
        }
        readTimeAndShow( MODE_SHOW_TIME );
      }
      break;
      
    case MODE_SET_YEAR:
      if( type == BUT_DOWN )
      {
        if( id == ID_LEFT )
        {
          flags= (flags ^ _BV(FLAG_INDEX)) | _BV(FLAG_FLASH);
        }
        else if( id == ID_RIGHT )
        {
          incYear( flags & _BV(FLAG_INDEX) );
          showTime( mode );
        }
      }
      else if( type == BUT_LONG )
      {
        if( id == ID_LEFT )
        {
          if(I2CMaster::isBusy())
          {
            setMode(MODE_ERROR);
            return;
          }
          ds1337.writeDate();
        }
        readTimeAndShow( MODE_SHOW_TIME );
      }
      break;
    
  }
}

void RetroUhr::run()
{
#ifdef DEBUG
  PRR= _BV(PRTIM2)|_BV(PRTIM0)|_BV(PRSPI)|_BV(PRADC);
#else
  PRR= _BV(PRTIM2)|_BV(PRTIM0)|_BV(PRSPI)|_BV(PRUSART0)|_BV(PRADC);
#endif  
  Segment::init();
  I2CMaster::init(64);
  Timer1::init( 
#ifdef DEBUG
               2
#else
               1
#endif
                , 14 );
  ICR1= 2000;
  sei();

  setMode(MODE_INIT);
  
  timeout= Timer1::getTickerWord();
  for(;;)
  {
    idle();
    
    buttonL.cyclic();
    buttonR.cyclic();
  
    WORD diff = Timer1::getTickerWord() - timeout;
    if(mode == MODE_INIT)
    {
      if( diff > 250 )
      {
        timeout= Timer1::getTickerWord();
        for(signed char i=0;i < 4;i++ )
        {
          BYTE s= 0;
          if(midx+i >= 0 &&  midx+i < (signed char)sizeof(message) )
            s= pgm_read_byte(&message[midx+i]);
            
          Segment::setDisplay( i, s );
          
        }
        
        if(++midx > (signed char)(sizeof(message)+3))
          setMode(MODE_POWERSAVE);
      }
    }
    else
    {
      diff = Timer1::getTickerWord() - timeout;
      WORD wait= 2500;
      switch( mode )
      {
        case MODE_SHOW_TIME:
        case MODE_SHOW_TIME_SEC:
        if( diff > 250 && !I2CMaster::isBusy() )
        {
          readTimeAndShow( mode );
        }
        break;
        case MODE_SET_TIME:
        case MODE_SET_DATE:
        case MODE_SET_YEAR:
        wait= 5000;
        if( diff > 150 )
        {
          flags ^= _BV(FLAG_FLASH);
          showTime( mode );
          timeout= Timer1::getTickerWord();
        }
        break;
      }
    
      if( mode != MODE_POWERSAVE )
      {
        diff= Timer1::getTickerWord() - powersaveTimeout;
        if( diff > wait )
        setMode(MODE_POWERSAVE);
      }
    }
  }
}

void idle()
{
  
#ifdef DEBUG
  debugIdle();
#endif

}

ISR(INT0_vect)
{
  EIMSK &= ~(_BV(INT0)|_BV(INT1));
}

ISR(INT1_vect)
{
  EIMSK &= ~(_BV(INT0)|_BV(INT1));  
}

void powerSave()
{
  cli();
  
  EIFR |= _BV(INTF1)|_BV(INTF0);
  EICRA= 0;
  EIMSK |= _BV(INT0)|_BV(INT1);
  
  sei();
  SMCR= 0;
  
#ifdef DEBUG
  LED1_ON();
#endif

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_mode();    
  
  EIMSK &= ~(_BV(INT0)|_BV(INT1));  
  
#ifdef DEBUG
  LED1_OFF();
#endif
}
