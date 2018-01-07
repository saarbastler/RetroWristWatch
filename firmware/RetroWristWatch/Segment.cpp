/*
 * Segment.cpp
 *
 * Created: 01.08.2016 18:23:09
 *  Author: Joerg
 */ 

#include <avr/pgmspace.h>
#include <avr/io.h>

#include "Segment.h"
#include "Timer1.h"
#include "DelayTimerW.h"

//DelayTimerWord mux( Timer1::getTickerAddr(), 5 );

const BYTE segment [] PROGMEM= { 0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x77, 0x7c, 0x58, 0x5e, 0x79, 0x71  };
  
volatile BYTE digit[4] asm("Segment_digit");
volatile BYTE digitIdx asm("Segment_digitIdx");
volatile BYTE digitFlags asm("Segment_digitFlags");

//static BYTE digit[4];
//static BYTE digitIdx;

void Segment::init()
{
  DDRB= PORTB_BITS;
  DDRC= PORTC_BITS;
  DDRD= PORTD_BITS;
  PORTB &= ~PORTB_BITS;
  PORTC &= ~PORTC_BITS;
  PORTD &= ~PORTD_BITS;
  
  digit[0]= 0;
  digit[1]= 0;
  digit[2]= 0;
  digit[3]= 0;
  
  digitFlags= 0;
  digitIdx = 0;
  //mux.start();
}

void Segment::enableDisplay( BYTE v )
{
  digitFlags= v;
  if(!digitFlags)
  {
    DIGIT1_PORT &= ~_BV(DIGIT1);
    DIGIT2_PORT &= ~_BV(DIGIT2);
    DIGIT3_PORT &= ~_BV(DIGIT3);
    DIGIT4_PORT &= ~_BV(DIGIT4);
  }
}
/*
void setDigit(BYTE digit, BYTE on)
{
  switch( digit & 3 )
  {
    case 0:
      if(on)
        DIGIT1_PORT |= _BV(DIGIT1);
      else
        DIGIT1_PORT &= ~_BV(DIGIT1);
      break;
    case 1:
      if(on)
        DIGIT2_PORT |= _BV(DIGIT2);
      else
        DIGIT2_PORT &= ~_BV(DIGIT2);
      break;
    case 2:
      if(on)
        DIGIT3_PORT |= _BV(DIGIT3);
      else
        DIGIT3_PORT &= ~_BV(DIGIT3);
      break;
    case 3:
      if(on)
        DIGIT4_PORT |= _BV(DIGIT4);
      else
        DIGIT4_PORT &= ~_BV(DIGIT4);
      break;
  }
}

void setSegments( BYTE bits )
{
  if( bits & 0x01 )  
    SEGMENT_A_PORT |= _BV(SEGMENT_A);
  else
    SEGMENT_A_PORT &= ~_BV(SEGMENT_A);
  if( bits & 0x02 )
    SEGMENT_B_PORT |= _BV(SEGMENT_B);
  else
    SEGMENT_B_PORT &= ~_BV(SEGMENT_B);
  if( bits & 0x04 )
    SEGMENT_C_PORT |= _BV(SEGMENT_C);
  else
    SEGMENT_C_PORT &= ~_BV(SEGMENT_C);
  if( bits & 0x08 )
    SEGMENT_D_PORT |= _BV(SEGMENT_D);
  else
    SEGMENT_D_PORT &= ~_BV(SEGMENT_D);
  if( bits & 0x10 )
    SEGMENT_E_PORT |= _BV(SEGMENT_E);
  else
    SEGMENT_E_PORT &= ~_BV(SEGMENT_E);
  if( bits & 0x20 )
    SEGMENT_F_PORT |= _BV(SEGMENT_F);
  else
    SEGMENT_F_PORT &= ~_BV(SEGMENT_F);
  if( bits & 0x40 )
    SEGMENT_G_PORT |= _BV(SEGMENT_G);
  else
    SEGMENT_G_PORT &= ~_BV(SEGMENT_G);
  if( bits & 0x80 )
    SEGMENT_DP_PORT |= _BV(SEGMENT_DP);
  else
    SEGMENT_DP_PORT &= ~_BV(SEGMENT_DP);
  
}

void Segment::cyclic()
{
  if(mux.isDone())
  {
    setDigit( digitIdx++, 0);
    setSegments( digit[digitIdx & 3]);
    setDigit( digitIdx, 1);
    mux.start();
  } 
}*/

void Segment::setDisplay( BYTE index, BYTE bits )
{
  digit[index & 3]= bits;
}

void Segment::setDisplayDigit( BYTE index, BYTE digit )
{
  digit &= 0x0f;
  BYTE bits= pgm_read_byte(&segment[digit]);
  setDisplay( index, bits);
}

void Segment::setDP( BYTE index, BYTE on)
{
  index &= 3;
  if(on)
    digit[index] |= 0x80;
  else
    digit[index] &= ~0x80;
}
  
void Segment::setHex(WORD value )
{
  setDisplayDigit( 3, value );
  value >>= 4;
  setDisplayDigit( 2, value );
  value >>= 4;
  setDisplayDigit( 1, value );
  value >>= 4;
  setDisplayDigit( 0, value );
}
