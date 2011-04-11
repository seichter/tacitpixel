/* 

    libTAP - The Architectural Playground 
    a minimalistic collaborative virtual environment framework
    
    Copyright (C) 1999-2003 Hartmut Seichter 

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA


*/ 

#ifndef TPSERIALLINE_H
#define TPSERIALLINE_H


/*! \file tpSerialLine.h
 *  \brief a RS232 wrapper
 */


#include <tp/globals.h>
#include <tp/types.h>
#include <tp/string.h>


#if defined(__unix) || defined(__APPLE__)
#if defined(__sgi)
  #include <term.h>
#endif
  #include <termios.h>
  #include <sys/ioctl.h>
#endif

#ifdef _WIN32
 #define B9600		9600
 #define B19200		19200
 #define B38400		38400
 #define B57600		57600
 #define B115200	115200
#endif

// Flow control flags
#define FC_DTRDSR       0x01
#define FC_RTSCTS       0x02
#define FC_XONXOFF      0x04

// ASCII definitions
#define ASCII_BEL       0x07
#define ASCII_BS        0x08
#define ASCII_LF        0x0A
#define ASCII_CR        0x0D
#define ASCII_XON       0x11
#define ASCII_XOFF      0x13

/*!
  \class tpSerialLine
  \brief a abstraction for a serial line
  \version 1.0
  
  This class provides main functionality for
  the serial line configuration. It is based on
  some code from Jan Springer <jsd@igroup.org>
*/

class TP_API tpSerialLine 
{  
public:

	tpSerialLine();
	tpSerialLine(const tpString& tty);
	virtual ~tpSerialLine();

	// settings
	int setVmin(int vmin);
	int setVtime(int vtime);
	int setBaudrate(int baud_rate);
	int setXon(int on);
	int setXoff(int on);
	int setStopbits(int);
	int setBytesize(int = 8);
	int setParity(int = 0);
	int setFlowControl(unsigned char);
	int useTimeout(bool=true);
	int setOutputBufferSize(int=0);
	int setInputBufferSize(int=0);

	// open control
	int open(const tpString& ttyname);
	int close();
	int reopen();
	int isOpen() const;

	// read access
	int readChar(char& c);
	int read(char *buf, int bytes);

	// write access
	int writeChar(char c);
	int write(const char* buf, int bytes);
	int writeString(const char* buf);

	void flushInput();
	void flushOutput();


	int convert2Baud(int);

	tpHandle getFD() const;

private:
  
	int configLine();
  

	tpHandle m_fd;


#if defined(_WIN32)
	// OVERLAPPED _offsR, _offsW;
#endif


	unsigned char m_flowcontrol;


	int  m_parity;
	int  m_bytesize;
	int  m_baud_rate;

	int  m_vmin;
	int  m_vtime;
	int  m_xon;
	int  m_xoff;
	bool m_use_two_stopbits;
	bool m_use_timeout;




	int   m_use_outputbuffer;
	int   m_use_inputbuffer;


	tpString m_ttyname;
};



#endif
