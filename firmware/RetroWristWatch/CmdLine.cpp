/*
 * CmdLine.cpp
 *
 * Created: 03.06.2015 18:55:34
 *  Author: Jörg
 */ 

#include "CmdLine.h"

CmdLine::CmdLine( char *_lineBuffer, BYTE _size, PrintStreamBase *_echo ) 
: lineBuffer(_lineBuffer), size(_size), echo(_echo)
{
  index= 0;
}

void CmdLine::appendChar( char ch )
{
  if( ch == 13 )
  {
    lineBuffer[index]= 0;
    evaluateLine(); 
  }
  else if( ch == 8 )
  {
    if( index > 0 )
    {
      index--;
      if( echo != 0 )
      {
        echo->putch( ch );
        echo->putch( ' ' );
        echo->putch( ch );
      }      
    }    
  }
  else if( ((BYTE)ch) >= ' ' && index < size-1 )
  {
    lineBuffer[index++]= ch;
    if( echo != 0 )
    {
      echo->putch( ch );
    }        
  }
}

void CmdLine::evaluateLine()
{
  index = 0;
  evaluateIndex= 0;
  evaluateError= 0;
  
  if( echo != 0 )
  {
    echo->putch( 13 );
    echo->putch( 10 );
  }
  if( !executeCommand( nextCharIgnoreBlank() ) )
  {
    printError( PSTR("???"));
  }
}

void CmdLine::printError( const char *string )
{
  if( echo != 0 )
  {
    for(BYTE i=1;i < evaluateIndex; i++ )
    {
      echo->putch( '-' );
    }
    echo->putch( '^' );
    echo->putch( 13 );
    echo->putch( 10 );
    echo->crlf();
    echo->print_P( string );
    echo->crlf();
  }
}

BYTE CmdLine::executeCommand( char ch )
{
  return 0;
}

void CmdLine::charBack()
{
  if( evaluateIndex > 0 )  
  {
    evaluateIndex--;
  }
}

char CmdLine::nextChar()
{
  char ch= 0;
  if( evaluateIndex < size ) 
  {
    ch= lineBuffer[ evaluateIndex ];
    if( ch != 0 )
    {
      evaluateIndex++;
    }
  }
  
  return ch;
}

char CmdLine::nextCharIgnoreBlank()
{
  char ch= nextChar();
  
  while( ch == ' ' || ch == 9 )
  {
    ch= nextChar();
  }
  
  return ch;
}

BYTE CmdLine::fromDez( char ch )
{
  if( ch < '0' )
  {
    return 0xff;
  }
  else if( ch <= '9' )
  {
    return ch -'0';
  }
  else
  {
    return 0xff;
  }
}

BYTE CmdLine::fromHex( char ch )
{
  if( ch < '0' )
  {
    return 0xff;
  }
  else if( ch <= '9' )
  {
    return ch -'0';
  }
  else if( ch < 'A' )
  {
    return 0xff;
  }
  else if( ch <= 'F' )
  {
    return 10+ ch - 'A';
  }
  else if( ch < 'a' )
  {
    return 0xff;
  }
  else if( ch <= 'f' )
  {
    return 10+ ch - 'a';
  }
  else
  {
    return 0xff;
  }
}

int CmdLine::nextDecWord()
{
  WORD result= 0;
  evaluateError= 0;
  BYTE minus= 0;
  
  char ch= nextCharIgnoreBlank();
  if( ch == '-' )
  {
    minus= 1;
    ch= nextCharIgnoreBlank();
  }
  
  ch= fromDez( ch );
  if( ch == (char)0xff )
  {
    evaluateError=1;
    return 0;
  }
  
  do
  {
    if( (minus && result > 3276) || (!minus && result > 6553 ) )
    {
      evaluateError=1;
      return 0;
    }
    else if( (minus && result == 3276 && ch > 8) || (!minus && result == 6553 && ch > 5) )
    {
      evaluateError=1;
      return 0;
    }
    
    result *= 10;
    result += ch;
    
    ch= fromDez( lineBuffer[evaluateIndex] );
    if( ch != (char)0xff )
    {
      nextChar();
    }
  }
  while( ch != (char)0xff );
  
  if( minus )
  {
    result= -result;
  }
  
  return result;
}

int CmdLine::nextDecimalFixpoint()
{
  int result= nextDecWord();
  
  char ch= nextChar();
  if( ch == '.')
  {
    BYTE i;
    for(i=0;i < 2;i++)
    {
      if( result > 3276 || result < -3276)
      {
        evaluateError=1;
        return 0;
      }
      result *= 10;
  
      ch= nextChar();
      BYTE v= fromDez( ch );
      if( v == 0xff )
      {
        charBack();
      }
      else
      {
        if( result < 0 )
        {
          result -= v;
        }
        else
        {
          result += v;
        }        
      }    
    }    
  }
  else
  {
    charBack();
    result *= 100;
  }
  
  return result;
}

WORD CmdLine::nextHexWord()
{
  WORD result= 0;
  evaluateError= 0;
  
  char ch= fromHex( nextCharIgnoreBlank() );
  if( ch == (char)0xff )
  {
    evaluateError=1;
    return 0;
  }
  
  do
  {
    if( result & 0xf000 )
    {
      evaluateError=1;
      return 0;
    }
    
    result <<= 4;
    result |= ch;
    
    ch= fromHex( lineBuffer[evaluateIndex] );
    if( ch != (char)0xff )
    {
      nextChar();
    }
  }
  while( ch != (char)0xff );
  
  return result;
}

DWORD CmdLine::nextHexDWord()
{
  DWORD result= 0;
  evaluateError= 0;
  
  char ch= fromHex( nextCharIgnoreBlank() );
  if( ch == 0xff )
  {
    evaluateError=1;
    return 0;
  }
  
  do
  {
    if( result & 0xf0000000 )
    {
      evaluateError=1;
      return 0;
    }
    
    result <<= 4;
    result |= ch;
    
    ch= fromHex( lineBuffer[evaluateIndex] );
    if( ch != 0xff )
    {
      nextChar();
    }
  }
  while( ch != 0xff );
  
  return result;
  
}
