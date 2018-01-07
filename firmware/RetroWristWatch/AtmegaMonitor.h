/*
 * AtmegaMonitor.h
 *
 * Created: 03.06.2015 19:50:54
 *  Author: Jörg
 */ 


#ifndef ATMEGAMONITOR_H_
#define ATMEGAMONITOR_H_

#include "device.h"
#include "CmdLine.h"

class AtmegaMonitor : public CmdLine
{
public:
  AtmegaMonitor(char *lineBuffer, BYTE size, PrintStreamBase *echo);
  
protected:

  // extension, return 1, if command is executed, 0 on error
  virtual BYTE executeCommand( char ch );
  
private: 
#ifdef ATMEGA_MONITOR_PORTS
  void dumpPorts();
  void dumpPort(char pch, BYTE pin, BYTE ddr, BYTE port);
#endif

#ifdef ATMEGA_MONITOR_CPU
  void dumpCPU();
#endif

#ifdef ATMEGA_MONITOR_MEMORY  
  void dumpMemory();
  
  WORD lastDumpAddr;
  WORD lastRows;
#endif
  
#ifdef ATMEGA_MONITOR_TIMER  
  void dumpTimers();
#if defined(OCR1C)
  void dumpTimer( char ti, BYTE tccra, BYTE tccrb, BYTE tccrc, WORD tcnt, WORD ocra, WORD ocrb, WORD ocrc, WORD icr);
#else
  void dumpTimer( char ti, BYTE tccra, BYTE tccrb, BYTE tccrc, WORD tcnt, WORD ocra, WORD ocrb, WORD icr);
#endif

  void dumpTimer8( char ti, BYTE tccra, BYTE tccrb, BYTE tcnt, BYTE ocra, BYTE ocrb);
#endif

#ifdef ATMEGA_MONITOR_USART
  void dumpUsart();
  void dumpUsart( char ch, BYTE ucsra, BYTE ucsrb, BYTE ucsrc, WORD ubbr );     
#endif

#ifdef ATMEGA_MONITOR_EEPROM
  void dumpEEProm();

  WORD lastEEPromsAddr;
  WORD lastEEPromRows;
#endif

};



#endif /* ATMEGAMONITOR_H_ */