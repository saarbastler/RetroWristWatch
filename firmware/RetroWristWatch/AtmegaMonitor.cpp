/*
 * AtmegaMonitor.cpp
 *
 * Created: 03.06.2015 19:51:58
 *  Author: Jörg
 */ 

#include <avr/eeprom.h>

#include "AtmegaMonitor.h"

#define LOWER_CASE(x)       (((x) >= 'A' && (x) <= 'Z') ? ((x) + 0x20) : (x))

/*static void cmdLineEcho( char ch )
{
  echo->putch( ch ) ;
}*/

AtmegaMonitor::AtmegaMonitor(char *_lineBuffer, BYTE _size, PrintStreamBase *echo) : CmdLine( _lineBuffer, _size, echo )
{
  
}


BYTE AtmegaMonitor::executeCommand( char ch )
{
  switch(ch)
  {
#ifdef ATMEGA_MONITOR_PORTS
    case ATMEGA_MONITOR_PORTS:
    dumpPorts();
    break;
#endif

#ifdef ATMEGA_MONITOR_TIMER
    case ATMEGA_MONITOR_TIMER:
    dumpTimers();
    break;
#endif

#ifdef ATMEGA_MONITOR_MEMORY
    case ATMEGA_MONITOR_MEMORY:
    dumpMemory();
    break;
#endif

#ifdef ATMEGA_MONITOR_EEPROM
    case ATMEGA_MONITOR_EEPROM:
    dumpEEProm();
    break;
#endif

#ifdef ATMEGA_MONITOR_CPU
    case ATMEGA_MONITOR_CPU:
    dumpCPU();
    break;
#endif

#ifdef ATMEGA_MONITOR_USART
    case ATMEGA_MONITOR_USART:
    dumpUsart();
    break;
#endif

#ifdef ATMEGA_MONITOR_I2C
    case ATMEGA_MONITOR_I2C:
    echo->printf_P( PSTR( "TWBR:%bx TWCR:%bx TWSR:%bx TWDR:%bx TWAR:%bx TWAMR:%bx\r\n"),
    TWBR, TWCR, TWSR, TWDR, TWAR, TWAMR );
    break;
#endif

    default:
      return 0;
  }
  
  return 1;
}

#ifdef ATMEGA_MONITOR_PORTS
void AtmegaMonitor::dumpPorts()
{
  #define MODE_SETBIT 0
  #define MODE_RESET  1
  #define MODE_SET    2
  #define HANDLE_PORT(PORT)   \
  if( mode & _BV(MODE_RESET)) \
  {                           \
    PORT &= b;               \
  }                           \
  if( mode & _BV(MODE_SETBIT))\
  {                           \
    PORT |= b;               \
  }                           \
  if( mode & _BV(MODE_SET))   \
  {                           \
    PORT = b;                \
  }                           \

  char ch= nextCharIgnoreBlank();
  ch= LOWER_CASE( ch );
  
  BYTE b= 0;
  char mode= nextCharIgnoreBlank();
  mode= LOWER_CASE( mode );
  if( mode != 0 )
  {
    b= nextHexWord();
    if( evaluateError )
    {
      mode= 0;
    }
    else if( mode == 's' )
    {
      mode= _BV(MODE_SETBIT);
      b= 1 << (7 & b);
    }
    else if( mode == 'r' )
    {
      mode= _BV(MODE_RESET);
      b= ~(1 << (7 & b));
    }
    else if( mode == '=' )
    {
      mode= _BV(MODE_SET);
    }
    else
    {
      mode= 0;
    }
  }
  
  #if defined(PORTA) && defined(PINA) && defined (DDRA)
  if( ch == 'a' )
  {
    HANDLE_PORT( PORTA );
  }
  if( ch == 0 || ch == 'a' )
  {
    dumpPort('A', PINA, DDRA, PORTA );
  }
  #endif
  #if defined(PORTB) && defined(PINB) && defined (DDRB)
  if( ch == 'b' )
  {
    HANDLE_PORT( PORTB );
  }
  if( ch == 0 || ch == 'b' )
  {
    dumpPort('B', PINB, DDRB, PORTB );
  }
  #endif
  #if defined(PORTC) && defined(PINC) && defined (DDRC)
  if( ch == 'c' )
  {
    HANDLE_PORT( PORTC );
  }
  if( ch == 0 || ch == 'c' )
  {
    dumpPort('C', PINC, DDRC, PORTC );
  }
  #endif
  #if defined(PORTD) && defined(PIND) && defined (DDRD)
  if( ch == 'd' )
  {
    HANDLE_PORT( PORTD );
  }
  if( ch == 0 || ch == 'd' )
  {
    dumpPort('D', PIND, DDRD, PORTD );
  }
  #endif
  #if defined(PORTE) && defined(PINE) && defined (DDRE)
  if( ch == 'e' )
  {
    HANDLE_PORT( PORTE );
  }
  if( ch == 0 || ch == 'e' )
  {
    dumpPort('E', PINE, DDRE, PORTE );
  }
  #endif
  #if defined(PORTF) && defined(PINF) && defined (DDRF)
  if( ch == 'f' )
  {
    HANDLE_PORT( PORTF );
  }
  if( ch == 0 || ch == 'f' )
  {
    dumpPort('F', PINF, DDRF, PORTF );
  }
  #endif
  #if defined(PORTG) && defined(PING) && defined (DDRG)
  if( ch == 'g' )
  {
    HANDLE_PORT( PORTG );
  }
  if( ch == 0 || ch == 'g' )
  {
    dumpPort('G', PING, DDRG, PORTG );
  }
  #endif
  #if defined(PORTH) && defined(PINH) && defined (DDRH)
  if( ch == 'h' )
  {
    HANDLE_PORT( PORTH );
  }
  if( ch == 0 || ch == 'h' )
  {
    dumpPort('H', PINH, DDRH, PORTH );
  }
  #endif
  #if defined(PORTJ) && defined(PINJ) && defined (DDRJ)
  if( ch == 'j' )
  {
    HANDLE_PORT( PORTJ );
  }
  if( ch == 0 || ch == 'j' )
  {
    dumpPort('J', PINJ, DDRJ, PORTJ );
  }
  #endif
  #if defined(PORTK) && defined(PINK) && defined (DDRK)
  if( ch == 'k' )
  {
    HANDLE_PORT( PORTK );
  }
  if( ch == 0 || ch == 'k' )
  {
    dumpPort('K', PINK, DDRK, PORTK );
  }
  #endif
  #if defined(PORTL) && defined(PINL) && defined (DDRL)
  if( ch == 'l' )
  {
    HANDLE_PORT( PORTL );
  }
  if( ch == 0 || ch == 'l' )
  {
    dumpPort('L', PINL, DDRL, PORTL );
  }
  #endif
}

void AtmegaMonitor::dumpPort(char pch, BYTE pin, BYTE ddr, BYTE port)
{
  echo->printf_P( PSTR("PIN%ch: %bx DDR%ch: %bx PORT%ch: %bx\r\n"), pch, pin, pch, ddr, pch, port );
}
#endif

#ifdef ATMEGA_MONITOR_TIMER

#define DUMP_REGISTER( prefix, no, suffix, reg)       \
echo->print_P( PSTR( prefix ));                      \
echo->putch( '0' + no );                             \
echo->putch( suffix );                               \
echo->print_P( PSTR(": "));                          \
echo->printHex( reg );                               \
echo->putch( ' ' )                                   \

void AtmegaMonitor::dumpTimers( )
{
  char ch= nextCharIgnoreBlank();
  ch= LOWER_CASE( ch );

  #if defined(TCCR0A)
  if( ch == 0 || ch == '0' )
  {
    dumpTimer8('0', TCCR0A, TCCR0B, TCNT0, OCR0A, OCR0B );
  }
  #endif
  #if defined(TCCR1A)
  if( ch == 0 || ch == '1' )
  {
    dumpTimer('1', TCCR1A, TCCR1B, TCCR1C, TCNT1, OCR1A, OCR1B,
    #if defined(OCR1C)
    OCR1C,
    #endif
    ICR1 );
  }
  #endif
  #if defined(TCCR2A)
  if( ch == 0 || ch == '2' )
  {
    dumpTimer8('2', TCCR2A, TCCR2B, TCNT2, OCR2A, OCR2B );
  }
  #endif
  #if defined(TCCR3A)
  if( ch == 0 || ch == '3' )
  {
    dumpTimer('3', TCCR3A, TCCR3B, TCCR3C, TCNT3, OCR3A, OCR3B,
    #if defined(OCR3C)
    OCR3C,
    #endif
    ICR3 );
  }
  #endif
  #if defined(TCCR4A)
  if( ch == 0 || ch == '4' )
  {
    DUMP_REGISTER( "TCCR", 4, 'A', TCCR4A );
    DUMP_REGISTER( "TCCR", 4, 'B', TCCR4B );
    DUMP_REGISTER( "TCCR", 4, 'C', TCCR4C );
    #ifdef TCCR4D
    DUMP_REGISTER( "TCCR", 4, 'D', TCCR4D );
    #endif
    DUMP_REGISTER( "OCR", 4, 'A', (WORD)OCR4A );
    DUMP_REGISTER( "OCR", 4, 'B', (WORD)OCR4B );
    #ifdef OCR4C
    DUMP_REGISTER( "OCR", 4, 'C', (WORD)OCR4C );
    #endif
    #ifdef OCR4D
    DUMP_REGISTER( "OCR", 4, 'D', OCR4D );
    #endif
    #ifdef ICR4
    DUMP_REGISTER( "ICR", 4, ' ', (WORD)ICR4 );
    #endif
    DUMP_REGISTER( "TCNT4", 4, ' ', (WORD)TCNT4 );
    echo->crlf();
    //dumpTimer('4', TCCR4A, TCCR4B, TCCR4C, TCNT4, OCR4A, OCR4B, OCR4C , ICR4 );
  }
  #endif
  #if defined(TCCR5A)
  if( ch == 0 || ch == '5' )
  {
    dumpTimer('5', TCCR5A, TCCR5B, TCCR5C, TCNT5, OCR5A, OCR5B, OCR5C , ICR5 );
  }
  #endif
}

#if defined(OCR1C)
void AtmegaMonitor::dumpTimer( char ti, BYTE tccra, BYTE tccrb, BYTE tccrc, WORD tcnt, WORD ocra, WORD ocrb, WORD ocrc, WORD icr )
#else
void AtmegaMonitor::dumpTimer( char ti, BYTE tccra, BYTE tccrb, BYTE tccrc, WORD tcnt, WORD ocra, WORD ocrb, WORD icr )
#endif
{
  echo->printf_P( PSTR("TCCR%chA: %bx TCCR%chB: %bx TCCR%chC: %bx TCNT%ch: %wx\r\n")
  , ti, tccra, ti, tccrb, ti, tccrc, ti, tcnt );
  #if defined(OCR1C)
  echo->printf_P( PSTR("OCR%chA: %wx OCR%chB: %wx OCR%chC: %wx ICR%ch: %wx\r\n")
  , ti, ocra, ti, ocrb, ti, ocrc, ti, icr );
  #else
  echo->printf_P( PSTR("OCR%chA: %wx OCR%chB: %wx ICR%ch: %wx\r\n")
  , ti, ocra, ti, ocrb, ti, icr );
  #endif
}

void AtmegaMonitor::dumpTimer8( char ti, BYTE tccra, BYTE tccrb, BYTE tcnt, BYTE ocra, BYTE ocrb)
{
  echo->printf_P( PSTR("TCCR%chA: %bx TCCR%chB: %bx TCNT%ch: %bx\r\n")
  , ti, tccra, ti, tccrb, ti, tcnt );

  echo->printf_P( PSTR("OCR%chA: %bx OCR%chB: %bx\r\n")
  , ti, ocra, ti, ocrb);
}
#endif

#ifdef ATMEGA_MONITOR_MEMORY
void AtmegaMonitor::dumpMemory()
{
  WORD start= nextHexWord();
  WORD rows;
  if( evaluateError )
  {
    start= lastDumpAddr;
    rows= lastRows;
  }
  else
  {
    lastDumpAddr= start;
    rows= nextHexWord();
    if( evaluateError )
    {
      rows= lastRows;
    }
    else
    {
      lastRows= rows;
    }
  }
  
  if( rows == 0 )
  {
    rows= 1;
  }
  
  for( ;rows > 0;rows--)
  {
    echo->printHex( start );
    for(BYTE i=16;i != 0;i--)
    {
      echo->putch( ' ' );
      echo->printHex( *(BYTE *)start );
      start++;
    }
    echo->crlf();
  }
}
#endif

#ifdef ATMEGA_MONITOR_EEPROM
void AtmegaMonitor::dumpEEProm()
{
  WORD start= nextHexWord();
  WORD rows;
  if( evaluateError )
  {
    start= lastEEPromsAddr;
    rows= lastEEPromRows;
  }
  else
  {
    lastEEPromsAddr= start;
    rows= nextHexWord();
    if( evaluateError )
    {
      rows= lastEEPromRows;
    }
    else
    {
      lastEEPromRows= rows;
    }
  }
  
  if( rows == 0 )
  {
    rows= 1;
  }
  
  for( ;rows > 0;rows--)
  {
    echo->printHex( start );
    for(BYTE i=16;i != 0;i--)
    {
      echo->putch( ' ' );
      echo->printHex( eeprom_read_byte( (const uint8_t *)start ) );
      start++;
    }
    echo->crlf();
  }

}
#endif

#ifdef ATMEGA_MONITOR_CPU
void AtmegaMonitor::dumpCPU()
{
  echo->printf_P( PSTR( "SREG: %bx SP:%wx\r\n"), SREG, SP );
}
#endif

#ifdef ATMEGA_MONITOR_USART
void AtmegaMonitor::dumpUsart()
{
  #ifdef UCSR0A
  dumpUsart( '0', UCSR0A, UCSR0B, UCSR0C, UBRR0 );
  #endif
  #ifdef UCSR1A
  dumpUsart( '1', UCSR1A, UCSR1B, UCSR1C, UBRR1 );
  #endif
}

void AtmegaMonitor::dumpUsart( char ch, BYTE ucsra, BYTE ucsrb, BYTE ucsrc, WORD ubbr )
{
  echo->printf_P( PSTR("UCSR%chA: %bx UCSR%chB: %bx UCSR%chC: %bx UBBR%ch: %wx\r\n" )
  , ch, ucsra, ch, ucsrb, ch, ucsrc, ch, ubbr );
}
#endif
