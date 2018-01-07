#include "PrintStream.h"

PrintStream::PrintStream(pfPutch _putchFunc)
{
  putchFunc= _putchFunc;
}

PrintStream::PrintStream(pfPutchChar _putchFunc)
{
  putchFunc= (pfPutch)_putchFunc;
}

PrintStream::PrintStream(ByteReceiver *_byteReceiver) : byteReceiver(_byteReceiver)
{  
}

void PrintStream::putch( BYTE ch ) /*const*/
{
  if( putchFunc != 0 )
  {
    putchFunc( ch );    
  }
  if( byteReceiver != 0 )
  {
    byteReceiver->received( ch );
  }
}

