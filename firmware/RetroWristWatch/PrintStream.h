#ifndef PRINT_STREAM_INCLUDED
#define PRINT_STREAM_INCLUDED

#include "PrintStreamBase.h"
#include "ByteReceiver.h"

typedef void (*pfPutch)( BYTE ch );
typedef void (*pfPutchChar)( char ch );

class PrintStream : public PrintStreamBase
{
public:
	PrintStream(pfPutch _putchFunc);
	PrintStream(pfPutchChar _putchFunc);
  PrintStream(ByteReceiver *byteReceiver);
  
  virtual void putch( BYTE ch ) /*const*/;

protected:
  
  ByteReceiver *byteReceiver;
  pfPutch putchFunc;
};

#endif

