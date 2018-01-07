/*
 * CmdLine.h
 *
 * Created: 03.06.2015 18:53:22
 *  Author: Jörg
 */ 


#ifndef CMDLINE_H_
#define CMDLINE_H_

#include "integer.h"
#include "PrintStreamBase.h"
#include "ByteReceiver.h"

class CmdLine : public ByteReceiver
{
public:

  CmdLine( char *lineBuffer, BYTE size, PrintStreamBase *echo );
  
  virtual void received( BYTE data )  { appendChar(data); };
  void appendChar( char ch );
    
  void charBack();              // put last char back into buffer
  char nextChar();              // get next char, maybe 0, if end is reached
  char nextCharIgnoreBlank();   // get next char skip blanks

  // interpret next up to 6 chars as a decimal number [-]ddddd
  // call hasEvaluateError() to test on errors
  int nextDecWord();
  
  // interpret next up to 6 chars as a decimal number [-]ddd[.dd]
  // 123.45 will return 12345, 321 will return 32100
  // call hasEvaluateError() to test on errors
  int nextDecimalFixpoint();
  
  // interpret next up to 4 chars as a hex number hhhh
  // call hasEvaluateError() to test on errors
  WORD nextHexWord();
  
  // interpret next up to 8 chars as a hex number hhhhhhhh
  // call hasEvaluateError() to test on errors
  DWORD nextHexDWord();
  
  BYTE hasEvaluateError()       { return evaluateError; }
    
  static BYTE fromDez( char ch );
  static BYTE fromHex( char ch );
  
  void printError( const char *string );
    
protected:

  virtual BYTE executeCommand( char ch );
  
  void evaluateLine();
  
  
  char *lineBuffer;
  BYTE size;
  PrintStreamBase *echo;
  
  BYTE index;
  BYTE evaluateIndex;
  BYTE evaluateError;
};



#endif /* CMDLINE_H_ */