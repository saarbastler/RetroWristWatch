/* 
* ButtonDebounce.cpp
*
* Created: 06.08.2016 11:41:34
* Author: Joerg
*/

#include <avr/io.h>

#include "ButtonDebounce.h"
#include "Device.h"

#define FLAG_CHANGE     0
#define FLAG_BUT_DOWN   1
#define FLAG_BUT_LONG   2

// default constructor
ButtonDebounce::ButtonDebounce(volatile BYTE *pin_, BYTE bitMask_, volatile WORD *ticker_, BYTE id_, PF_BUTTON pfButton_) 
 : pin(pin_), bitMask(bitMask_), ticker(ticker_), id(id_), pfButton(pfButton_)
{
  *(pin+1) &= ~bitMask;
  *(pin+2) |= bitMask;
  
  oldValue= bitMask;
  flags= 0;
} //ButtonDebounce

void ButtonDebounce::cyclic()
{
  BYTE button= *pin & bitMask;
  
  if( button != oldValue )
  {
    if( flags & _BV(FLAG_CHANGE) )
    {
      WORD diff= *ticker - time;
      if( diff > DEBOUNCE_TIME )
      {
        oldValue= button;
        flags &= ~_BV(FLAG_CHANGE);
        
        if( !button )
        {
          flags |= _BV(FLAG_BUT_DOWN);
          time= *ticker; 
        }
        else
        {
          flags &= ~_BV(FLAG_BUT_DOWN);
        }          
                  
        pfButton( id, button ? BUT_UP : BUT_DOWN);
      }      
    }
    else
    {
      time= *ticker;
      flags |= _BV(FLAG_CHANGE);      
    }
  }
  else 
  {
    flags &= ~_BV(FLAG_CHANGE);
    if( flags & _BV(FLAG_BUT_DOWN) )
    {
      WORD diff= *ticker - time;
      if( diff > BUT_LONG_TIME )
      {
        flags &= ~_BV(FLAG_BUT_DOWN);
        
        pfButton( id, BUT_LONG );
      }        
    }
  }  
}
