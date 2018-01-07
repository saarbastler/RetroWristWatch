/*
 * PrintStreamBase.cpp
 *
 * Created: 24.03.2014 18:53:24
 *  Author: Jörg
 */

#include "PrintStreamBase.h"

void PrintStreamBase::crlf() /*const*/
{
  putch( '\r' ) ;
  putch( '\n' ) ;
}

BYTE PrintStreamBase::printf(const char *fmt, ...) /*const*/
{
  va_list p;
  va_start( p, fmt );

  BYTE count = v_printf( fmt, false, p );

  va_end( p );

  return count;
}

BYTE PrintStreamBase::printf_P(const char *fmt, ...) /*const*/
{
  va_list p;
  va_start( p, fmt );

  BYTE count = v_printf( fmt, true, p );

  va_end( p );

  return count;
}

BYTE PrintStreamBase::v_printf(const char *fmt, bool rom, va_list p) /*const*/
{
  BYTE count= 0;
  char ch0, ch1, ch2;

  if( rom )
  {
    ch0 = pgm_read_byte( fmt++ );
    ch1 = pgm_read_byte( fmt++ );
    ch2 = pgm_read_byte( fmt++ );
  }
  else
  {
    ch0= *fmt++;
    ch1= *fmt++;
    ch2= *fmt++;
  }

  while( ch0 != 0)
  {
    bool skip3= true;

    if( ch0 == '%' )
    {
      if( ch1 == 'b' )
      {
        if( ch2 == 'x' )
        {
          printHex( (BYTE) va_arg( p, unsigned int ) );
          count += 2;
        }
        else if( ch2 == 'd' )
        {
          count += printDez( (BYTE) va_arg( p, unsigned int ) );
        }
        else
        {
          putch( ch0 );
          count++;
          skip3= false;
        }
      }
      else if( ch1 == 'w' )
      {
        if( ch2 == 'x' )
        {
          printHex( (WORD) va_arg( p, unsigned int ) );
          count += 4;
        }
        else if( ch2 == 'd' )
        {
          count += printDez( (WORD) va_arg( p, unsigned int ) );
        }
        else
        {
          putch( ch0 );
          count++;
          skip3= false;
        }
      }
      else if( ch1 == 'l' )
      {
        if( ch2 == 'x' )
        {
          printHex( (DWORD) va_arg( p, unsigned long ) );
          count += 8;
        }
        else
        {
          putch( ch0 );
          count++;
          skip3= false;
        }
      }
      else if( ch1 == 's' )
      {
        if( ch2 == 'r' )
        {
          print( (char *) va_arg( p, char *) );
        }
        else if( ch2 == 'f' )
        {
          print( (const char *) va_arg( p, const char *) );
        }
        else
        {
          putch( ch0 );
          count++;
          skip3= false;
        }
      }
      else if( ch1 == 'c' && ch2 == 'h' )
      {
        putch( (char) va_arg( p, int ) );
        count ++;
      }
      else
      {
        putch( ch0 );
        count++;
        skip3= false;
      }
    }
    else
    {
      putch( ch0 );
      count++;
      skip3= false;
    }

    if( skip3 )
    {
      if( rom )
      {
        ch0 = pgm_read_byte( fmt++ );
        ch1 = pgm_read_byte( fmt++ );
        ch2 = pgm_read_byte( fmt++ );
      }
      else
      {
        ch0= *fmt++;
        ch1= *fmt++;
        ch2= *fmt++;
      }
    }
    else
    {
      ch0= ch1;
      ch1= ch2;

      if( rom )
      {
        ch2= pgm_read_byte( fmt++ );
      }
      else
      {
        ch2= *fmt++;
      }
    }
  }
  return count;
}

BYTE PrintStreamBase::print(const char *string) /*const*/
{
  BYTE count= 0;

  while( *string )
  {
    putch( *string++ );
    count++;
  }

  return count;
}

BYTE PrintStreamBase::print_P(const char *string) /*const*/
{
  BYTE count= 0;
  char c;

  while( (c = pgm_read_byte( string++ )) != 0 )
  {
    putch( c );
    count++;
  }

  return count;
}

void PrintStreamBase::printHexnibble(BYTE c) /*const*/
{
  c &= 0xf;
  c |= '0';
  if(c > '9')
  c += 'A' - '9' -1;

  putch( (char) c );
}

void PrintStreamBase::printHex(BYTE c) /*const*/
{
  printHexnibble( c >> 4 );
  printHexnibble( c );
}

void PrintStreamBase::printHex(WORD w) /*const*/
{
  printHex( (BYTE)(w >> 8) );
  printHex( (BYTE)w );
}

void PrintStreamBase::printHex(DWORD dw) /*const*/
{
  printHex( (WORD)(dw >> 16) );
  printHex( (WORD)dw );
}

BYTE PrintStreamBase::printDez(BYTE b) /*const*/
{
  BYTE count= 1;

  if( b >= 100 )
  {
    putch( '0' + ( b / 100 ) );
    b %= 100;

    count++;
  }

  if( b >= 10 )
  {
    putch( '0' + (b / 10) );
    b %= 10;
  }

  putch( '0' + b );

  return count;
}

BYTE PrintStreamBase::printDez(WORD w) /*const*/
{
  BYTE count= 0;
  bool p= false;
  WORD div= 10000;

  for(;div > 0; div /= 10)
  {
    if( p || div == 1 || w >= div)
    {
      putch( '0' + ( w / div ) );
      w %= div;
      p= true;
      count++;
    }
  }

  return count;
}

BYTE PrintStreamBase::printDez(WORD w, BYTE fieldSize) /*const*/
{
  BYTE count= 0;
  bool p= false;
  WORD div= 10000;

  while( fieldSize > 5 )
  {
    putch( ' ' );
    fieldSize--;
  }

  for(;div > 0; div /= 10)
  {
    if( p || div == 1 || w >= div)
    {
      putch( '0' + ( w / div ) );
      w %= div;
      p= true;
      count++;
    }
    else
    {
      if( 5-count <= fieldSize )
      {
        count++;
        fieldSize--;
        putch( ' ' );
      }
    }

  }

  return count;

}

void PrintStreamBase::dumpMemory( const char *prefix, BYTE *ptr, BYTE size ) /*const*/
{
  if( prefix != 0 )
  {
    print_P( prefix );
  }

  for( BYTE i=0;i < size; i++ )
  {
    printHex( *ptr++ );
    putch(' ');
  }

  crlf();
}
