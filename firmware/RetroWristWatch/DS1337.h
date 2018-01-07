/*
 * DS1337.h
 *
 * Created: 03.08.2016 18:48:57
 *  Author: Joerg
 */ 

#include "integer.h"
#include "I2CDevice.h"

#ifndef DS1337_H_
#define DS1337_H_

class DS1337 : public I2CDevice
{
public:
  DS1337(); 
  
  void readDateAndTime();
  void writeDateAndTime();
  
  void readDate();
  void writeDate();

  BYTE is24H();
  BYTE isPM();
  
  BYTE getSeconds();
  BYTE getMinutes();
  BYTE getHours();
  BYTE getDay();
  BYTE getDate();
  BYTE getMonth();
  WORD getYear();

  void set24H( BYTE b );
  void setPM( BYTE b );
  
  void setSeconds( BYTE b );
  void setMinutes( BYTE b );
  void setHours( BYTE b );
  void setDay( BYTE b );
  void setDate( BYTE b );
  void setMonth( BYTE b );
  void setYear( WORD w );
  
private:

  BYTE flags;
  BYTE buffer[16]; 
};



#endif /* DS1337_H_ */