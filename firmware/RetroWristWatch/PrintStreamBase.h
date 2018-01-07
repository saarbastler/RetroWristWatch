/*
 * PrintStreamBase.h
 *
 * Created: 24.03.2014 18:52:56
 *  Author: Jörg
 */


#ifndef PRINTSTREAMBASE_H_
#define PRINTSTREAMBASE_H_

#include <stdarg.h>
#include <avr/pgmspace.h>

#include "integer.h"

class PrintStreamBase
{
public:

	BYTE print(const char *string) /*const*/;
	BYTE print_P(const char *string) /*const*/;

	/*
	 * %ch = char
	 * %bx = Byte hex
	 * %wx = Word hex
	 * %lx = LWord hex
	 * %bd = Byte dez
	 * %wd = Word dez
   * %sr = string in ram
   * %sf = string in flash
	 */
	BYTE printf(const char *string, ...) /*const*/;
	BYTE printf_P(const char *string, ...) /*const*/;

	void printHexnibble(BYTE c) /*const*/;

	void printHex(BYTE c) /*const*/;
	void printHex(WORD w) /*const*/;
	void printHex(DWORD w) /*const*/;

	BYTE printDez(BYTE b) /*const*/;
	BYTE printDez(WORD w) /*const*/;

  BYTE printDez(WORD w, BYTE fieldSize) /*const*/;

  virtual void putch( BYTE ch ) /*const*/ = 0;
  void crlf() /*const*/;

  void dumpMemory( const char *prefix, BYTE *ptr, BYTE size ) /*const*/;

protected:

	BYTE v_printf(const char *fmt, bool rom, va_list p) /*const*/;
};



#endif /* PRINTSTREAMBASE_H_ */