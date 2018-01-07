/*
 * Segment.h
 *
 * Created: 01.08.2016 18:22:12
 *  Author: Joerg
 */ 


#ifndef SEGMENT_H_
#define SEGMENT_H_

#include "Device.h"

namespace Segment
{
  void init();
  
  //void cyclic();
  
  void setDisplay( BYTE index, BYTE bits );
  
  void setDisplayDigit( BYTE index, BYTE digit );
  
  void enableDisplay( BYTE v );
  
  void setDP( BYTE index, BYTE on);
  
  void setHex(WORD value );
}


#endif /* SEGMENT_H_ */