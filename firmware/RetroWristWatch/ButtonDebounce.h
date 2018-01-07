/* 
* ButtonDebounce.h
*
* Created: 06.08.2016 11:41:34
* Author: Joerg
*/


#ifndef __BUTTONDEBOUNCE_H__
#define __BUTTONDEBOUNCE_H__

#include "integer.h"

#define BUT_DOWN      0
#define BUT_UP        1
#define BUT_LONG      2

typedef void (*PF_BUTTON)(BYTE id, BYTE type);

class ButtonDebounce
{
//functions
public:
	ButtonDebounce(volatile BYTE *pin, BYTE bitMask, volatile WORD *ticker, BYTE id, PF_BUTTON pfButton);

  void cyclic();
  
private:

  volatile BYTE *pin;
  BYTE bitMask;
  
  volatile WORD *ticker;
  WORD time;
  
  BYTE oldValue;
  BYTE flags;
  
  BYTE id;
  PF_BUTTON pfButton;
}; //ButtonDebounce

#endif //__BUTTONDEBOUNCE_H__
