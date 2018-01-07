/*
 * DS1337.cpp
 *
 * Created: 03.08.2016 18:49:54
 *  Author: Joerg
 */ 

#include <avr/io.h>

#include "DS1337.h"

#define FLAG_12   _BV(6)
#define FLAG_PM   _BV(5)

DS1337::DS1337() : I2CDevice(0xd0)
{  
  flags= 0;
}

void DS1337::readDateAndTime()
{
  readRegister( 0, 7, buffer );  
}

void DS1337::writeDateAndTime()
{
  writeRegister(0, 7, buffer);
}

void DS1337::readDate()
{
  readRegister( 4, 3, buffer+4);
}

void DS1337::writeDate()
{
  writeRegister(4, 3, buffer+4);
}

BYTE DS1337::is24H()
{
  return !(flags & FLAG_12);
}

void DS1337::set24H( BYTE b )
{
  if(b)
    flags &= ~FLAG_12;
  else
    flags |= FLAG_12;
}

BYTE DS1337::isPM()
{
  return buffer[2] & FLAG_PM;
}

void DS1337::setPM( BYTE b )
{
  if(b)
    buffer[2] |= FLAG_PM;
  else
    buffer[2] &= ~FLAG_PM;
}

BYTE DS1337::getSeconds()
{
  return buffer[0];
}

BYTE DS1337::getMinutes()
{
  return buffer[1];
}

BYTE DS1337::getHours()
{
  BYTE h= buffer[2];
  
  set24H( !(h & 0x40));
   if( h & FLAG_12 )
    return h & 0x1f;
  else
    return h;
}

BYTE DS1337::getDay()
{
  return buffer[3];
}

BYTE DS1337::getDate()
{
  return buffer[4];
}

BYTE DS1337::getMonth()
{
  return buffer[5] & 0x1f;
}

WORD DS1337::getYear()
{
  if( buffer[5] & 0x80 )
    return 0x2100 | buffer[6];
  else
    return 0x2000 | buffer[6];
}

void DS1337::setSeconds( BYTE b )
{
  buffer[0]= b & 0x7f;
}

void DS1337::setMinutes( BYTE b )
{
  buffer[1]= b & 0x7f;
}

void DS1337::setHours( BYTE b )
{
  if( is24H() )
    buffer[2]= b & 0x3f;
  else
    buffer[2]= 0x40 | (b & 0x1f);
}

void DS1337::setDay( BYTE b )
{
  buffer[3]= b & 0x7;
}

void DS1337::setDate( BYTE b )
{
  buffer[4]= b & 0x3f;
}

void DS1337::setMonth( BYTE b )
{
  buffer[5]= (buffer[5] & 0x80) | (b & 0x1f);
}

void DS1337::setYear( WORD w )
{
  if( (w & 0xff) == 0x2100)
    buffer[5] |= 0x80;
  else
    buffer[5] &= ~0x80;
    
  buffer[6]= w & 0xff;
}
