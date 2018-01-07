/*
 * ByteReceiver.h
 *
 * Created: 14.06.2015 19:08:40
 *  Author: Jörg
 */ 


#ifndef BYTERECEIVER_H_
#define BYTERECEIVER_H_

#include "integer.h"

class ByteReceiver
{
public:
  
  virtual void received( BYTE data ) = 0;
};



#endif /* BYTERECEIVER_H_ */