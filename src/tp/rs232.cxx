#include "tp/rs232.h"

#include <errno.h>        // errno
#include <fcntl.h>        // O_* flags, ::open
#include <stdlib.h>     // ::free
#include <string.h>       // ::memset, ::strdup, ::strerror
#include <iostream>

#if defined(__unix) || defined(__APPLE__)
	#include <unistd.h>       // ::close, ::ioctl, ::read, ::write
	#include <sys/ioctl.h>
	#include <termios.h>
#elif defined(_WIN32)
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#endif

//
#include "tp/log.h"

#define NOTIMPL(s) cout << "Not yet implemented" << #s << endl


// ctor
tpRS232::tpRS232()
  : 
#if defined(__unix) || defined (__APPLE__)
    m_fd(-1),
#elif defined(_WIN32)
	m_fd(NULL),
#endif
	m_parity(0),
	m_bytesize(8),
    m_baud_rate(19200),
    m_vmin(0),
    m_vtime(0),
    m_xon(0),
    m_xoff(0),
    m_use_two_stopbits(false),
	m_use_timeout(false),
   
	m_use_outputbuffer(0),
	m_use_inputbuffer(0)
{


#if defined(_WIN32)
//   _offsW.Offset = 0 ;
//   _offsW.OffsetHigh = 0 ;
//   _offsR.Offset = 0 ;
//   _offsR.OffsetHigh = 0 ;
#endif


}



tpRS232::tpRS232(const tpString& tty)
  :
#if defined(__unix)
    m_fd(-1),
#elif defined(WIN32)
	m_fd(NULL),
#endif
	m_parity(0),
	m_bytesize(8),
    m_baud_rate(19200),
    m_vmin(0),
    m_vtime(0),
    m_xon(0),
    m_xoff(0),
    m_use_two_stopbits(false),
	m_use_timeout(false),

	m_use_outputbuffer(0),
	m_use_inputbuffer(0)
{ 


	m_ttyname = tty; 

  if (m_ttyname.isEmpty()) tpLogError("tpSerialLine() : tty descriptor is empty");
  else {
	   
	open(m_ttyname.c_str());
  }; 


}

tpRS232::~tpRS232()
{
	close();
}


int tpRS232::useTimeout(bool use) {

	m_use_timeout = use;
	return configLine();

}


int tpRS232::open(const tpString& ttyname) {

	if (ttyname.isEmpty())
  {

      tpLogError("tpSerialLine::open: no portname given!");   

    return 0;
  }


// UNIX
#if defined(__unix) || defined(__APPLE__)

  close();
  
  m_fd = ::open(ttyname.c_str(), O_RDWR);
  
  if (m_fd < 0) {



	  tpLogError("tpSerialLine::open: failed for [%s]",ttyname.c_str());


    m_fd = -1;
    
    return 0;
  }
  
// WINDOWS
#elif defined(_WIN32)


  tpLogMessage("tpSerialLine::open: [%s] will be opened",ttyname.c_str());


  m_fd = CreateFile(ttyname.c_str(), GENERIC_READ | GENERIC_WRITE, 
             0, 
             NULL, 
             OPEN_EXISTING, 
             FILE_FLAG_NO_BUFFERING, 
             NULL);

     // initializing serial line
  /*
  _fd = CreateFile(ttyname, GENERIC_READ | GENERIC_WRITE,
							FILE_SHARE_READ | FILE_SHARE_WRITE,
							NULL,
							OPEN_EXISTING,
							FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 
							NULL );
  */

  if (!m_fd) {

	  tpLogError("tpSerialLine::open: failed for [%s]",ttyname.c_str());

	  return 0;
  }  

  tpLogMessage("tpSerialLine::open: was succesfull for [%s]",ttyname.c_str());


#endif

   
   m_ttyname = ttyname;
  
  return configLine();
}



int tpRS232::isOpen() const
{

#if defined (__unix) || defined(__APPLE__)

  return (0 < m_fd);

#elif defined (_WIN32)

  return (m_fd != NULL);

#endif
}

int tpRS232::close()
{

#if defined(__unix) || (__APPLE__)
  if (isOpen()) {
    ::close(m_fd);
    
    m_fd = -1;
  }

  return (-1 == m_fd) ? 1 : 0;

#elif defined (_WIN32)


   SetCommMask(m_fd, 0) ;

   EscapeCommFunction(m_fd, CLRDTR ) ;

   PurgeComm( m_fd, PURGE_TXABORT | PURGE_RXABORT |
                   PURGE_TXCLEAR | PURGE_RXCLEAR );

   CloseHandle( m_fd );



  return 0;

#endif
}

int tpRS232::reopen()
{
	close();
	return open(m_ttyname.c_str());  
}

int tpRS232::writeChar(char c)
{
	return write(&c, 1);
}


int tpRS232::readChar(char& c)
{
  return read(&c, 1);
  
}

int tpRS232::read(char *buf, int bytes)
{

#if defined(__unix) || defined(__APPLE__)
  const int bytesread = ::read(m_fd, buf, bytes);
  

  if (bytesread != bytes) {

    tpLogError("tpSerialLine::read: could not read from [%s]!",m_ttyname.c_str());

  }
  
  return bytesread;
#elif defined(_WIN32)


/*
  BOOL    fReadStat ;
  COMSTAT ComStat ;
  DWORD   dwErrorFlags;
  DWORD   dwLength;
  DWORD   dwError;

  ClearCommError( _fd, &dwErrorFlags, &ComStat ) ;
  dwLength = min( (DWORD) bytes, ComStat.cbInQue ) ;

  if (dwLength > 0) {
    fReadStat = ReadFile( _fd, buf,
                          dwLength, &dwLength, &_offsR ) ;

    if (!fReadStat) {
      if (GetLastError() == ERROR_IO_PENDING) {
        if (_notify) cout << "\n\rIO Pending";

	while(!GetOverlappedResult( _fd, &_offsR, &dwLength, TRUE ))
	{
          dwError = GetLastError();

          if(dwError == ERROR_IO_INCOMPLETE) {
            continue;
          }
          else {
            if (_notify) cout << "tpSerialLine:read : error in read\n";
            ClearCommError( _fd, &dwErrorFlags, &ComStat ) ;
            break;
          }
	}
      }
      else {
        if (_notify) cout << "error in read\n";

        dwLength = 0 ;
		ClearCommError( _fd, &dwErrorFlags, &ComStat ) ;
      }
    }
  }

  */
   
  // return ( dwLength ) ;
  // return fReadStat;

  	DWORD	nRead;
	DCB		dcb;
	if (!ReadFile(m_fd, buf, bytes, &nRead, NULL)) {
		GetCommState(m_fd, &dcb);
		//return 0;
	}
	return (int)(nRead);


#endif

}

int tpRS232::write(const char *buf, int bytes)
{

#if defined(__unix) || defined(__APPLE__)
  const int byteswritten = ::write(m_fd, buf, bytes);
  
  if (byteswritten != bytes) {

      tpLogError("tpSerialLine::write: failed for [%s]",m_ttyname.c_str());

  }  
  return byteswritten;
#elif defined (WIN32)


   /*

  	DWORD		nWrite;
	DCB			dcb;
	DWORD		err;

	while (!WriteFile(_fd, buf, bytes, &nWrite, NULL)) {
		err = GetLastError();
		GetCommState(_fd, &dcb); 
	}
	return (int)(nWrite);

   */

  /*

  BOOL        fWriteStat ;
  DWORD       dwBytesWritten ;
  DWORD       dwErrorFlags;
  DWORD   	  dwError;
  COMSTAT     ComStat;

  fWriteStat = WriteFile( _fd, buf, bytes, &dwBytesWritten, &_offsW ) ;

  if (!fWriteStat) {
    if(GetLastError() == ERROR_IO_PENDING) {

      while(!GetOverlappedResult( _fd, &_offsW, &dwBytesWritten, TRUE ))
      {
        dwError = GetLastError();
        if(dwError == ERROR_IO_INCOMPLETE) {
          continue;
        }
		else {

			if (_notify) cout << "tpSerialLine::write: error in write\n";

          ClearCommError( _fd, &dwErrorFlags, &ComStat ) ;
          break;
        }
      }
    }
    else {						 	


	  if (_notify) cout << "tpSerialLine::write: error in write\n";

      ClearCommError( _fd, &dwErrorFlags, &ComStat ) ;
      return 0;
    }
  }

  return (int)fWriteStat;

  */

  	DWORD		nWrite;
	DCB			dcb;
	DWORD		err;

	while (!WriteFile(m_fd, buf, bytes, &nWrite, NULL)) {
		err = GetLastError();
		GetCommState(m_fd, &dcb); 
	}
	return (int)(nWrite);


#endif
}

int tpRS232::writeString(const char* buf)
{
  if (!buf) return 0;  
  return write(buf, ::strlen(buf));
}

void tpRS232::flushInput(){

#if defined (__unix)
#if !defined(__BSD__)
	if (-1 == ::ioctl(m_fd, TCFLSH, TCIFLUSH)) {
#else
	if (-1 == tcflush(m_fd,TCIFLUSH)) {
#endif
	tpLogError("tpSerialLine::flushOutput():  ioctl(TCFLSH, TCIFLSH) failed for %s with reason %s",m_ttyname.c_str(),::strerror(errno));
  };

#elif defined (_WIN32)

  COMMTIMEOUTS old;
  COMMTIMEOUTS tmp;
  char b;
  DWORD more = 1;
  int max = 1000;

  PurgeComm (m_fd, PURGE_RXABORT | PURGE_RXCLEAR);
  GetCommTimeouts (m_fd, &old);
  memset (&tmp, 0, sizeof (tmp));
  tmp.ReadTotalTimeoutConstant = 100;
  SetCommTimeouts (m_fd, &tmp);

  while (max > 0 && more) 
  {
    ReadFile (m_fd, &b, 1,  &more,  0);
    if (more) 
    {
      // --- Message ---
    } // if
	  max--;
  } // while
      
  SetCommTimeouts (m_fd, &old);


#endif
}


void tpRS232::flushOutput() {

	// UNIX
#if defined(__unix)
#if defined(__BSD__)
  if (-1 == ::tcflush(m_fd, TCOFLUSH))
#else
  if (-1 == ::ioctl(m_fd, TCFLSH, TCOFLUSH))
#endif

  {
    tpLogError("tpSerialLine::flushOutput(): ioctl(TCFLSH, TCOFLSH) failed for %s with reason %s",
       m_ttyname.c_str(), ::strerror(errno));
  }
  // WINDOWS
#elif defined(WIN32)

  PurgeComm (m_fd, PURGE_TXABORT | PURGE_TXCLEAR);

#endif
}


tpHandle tpRS232::getFD () const
{
  return m_fd;
}


int
tpRS232::setVmin(int vmin)
{
  m_vmin = vmin;
  
  return configLine();
}

int
tpRS232::setVtime(int vtime)
{
  m_vtime = vtime;
  
  return configLine();
}

int
tpRS232::setBaudrate(int baud_rate)
{
	m_baud_rate = baud_rate;
  
  return configLine();
}

int
tpRS232::setXon(int on)
{
  m_xon = on;
  
  return configLine();
}

int
tpRS232::setXoff(int on)
{
  m_xoff = on;
  
  return this->configLine();
}

int tpRS232::setStopbits(int bits)
{
	switch (bits){
	case 2:
		m_use_two_stopbits = true;
		break;
	default:
		m_use_two_stopbits = false;
		break;
	}
	return this->configLine();
}

int tpRS232::setBytesize(int bytes) {

	m_bytesize = bytes;

	return this->configLine();

}

int tpRS232::setParity(int parity) {

	m_parity = parity;

	return this->configLine();

}


int tpRS232::setFlowControl(unsigned char flowcontrol)
{
	m_flowcontrol = flowcontrol;
	return this->configLine();
	
}

int tpRS232::configLine() {

  
  if (!isOpen()) {

      tpLogMessage("tpSerialLine::configLine: could not change config, line [%s] not open",m_ttyname.c_str());
    
    return 0;
  }


#if defined(__unix) || defined (__APPLE__)

  struct termios _tio;
  
  ::memset(&_tio, 0, sizeof(_tio));
  
  int baud = convert2Baud(m_baud_rate);
  
  _tio.c_iflag = (m_xon ? IXON : 0) | (m_xoff ? IXOFF : 0) | IGNBRK | IGNPAR;;
  _tio.c_oflag = 0;
  _tio.c_lflag = 0;
  _tio.c_cflag = baud | CS8 | (m_use_two_stopbits ? CSTOPB : 0) | CREAD | HUPCL| CLOCAL;
  
  _tio.c_cc[VMIN] = m_vmin;	 // return with _vmin characters
  _tio.c_cc[VTIME] = m_vtime; // timeout after _vtime deciseconds

#if defined(__BSD__) || defined(__APPLE__)
  if (-1 == tcsetattr(m_fd, TCSANOW, &_tio)) {
#else
  if (-1 == ::ioctl(m_fd, TCSETA, &_tio)) {
#endif
      tpLogError("tpSerialLine::configLine: ioctl(TCSETA) failed for [%s] with error %s",
          m_ttyname.c_str(),::strerror(errno));

    
    return 0;
  }
  
  return 1;

#elif defined(_WIN32)
  
  if (!m_fd) {

      tpLogMessage("tpSerialLine::configLine: could not change config, [%s] not open",
          m_ttyname.c_str());

	  return 0;

  } else {


      tpLogMessage("tpSerialLine::configLine: [%s] with %d baud, %d bytesize, %d parity ",
          m_ttyname.c_str(),m_baud_rate,m_baud_rate,m_parity);



	COMMTIMEOUTS  CommTimeOuts ;

	  int err;

	  DCB dcb;
	  // BYTE bSet;
      if (!GetCommState(m_fd, &dcb )) 
          tpLogError("tpSerialLine::configLine could not get state for [%s]",m_ttyname.c_str());



	  dcb.BaudRate = m_baud_rate ;
	  dcb.ByteSize = m_bytesize;
	  dcb.Parity   = m_parity;


	  if (m_use_two_stopbits) dcb.StopBits = TWOSTOPBITS;
		else dcb.StopBits = ONESTOPBIT;

/*

	  bSet = (BYTE) ((_flowcontrol & FC_DTRDSR) != 0) ;

	  if (_notify && (bSet != 0)) cout << "tpSerialLine::configLine: DsrFlow On\n";

	  dcb.fOutxDsrFlow = bSet;

      if (bSet) {

		if (_notify) cout << "tpSerialLine::configLine: DTR_CONTROL_HANDSHAKE\n";
        dcb.fDtrControl = DTR_CONTROL_HANDSHAKE ;

	  } else {

		if (_notify) cout << "tpSerialLine::configLine: DTR_CONTROL_ENABLE\n";
        dcb.fDtrControl = DTR_CONTROL_ENABLE ;

	  };

      bSet = (BYTE) ((_flowcontrol & FC_RTSCTS) != 0);

	  if (_notify) cout << "tpSerialLine::configLine: CtsFlow " << bSet << "\n";
      
	  dcb.fOutxCtsFlow = bSet ;

      if (bSet) {

        if (_notify) cout << "tpSerialLine::configLine: RTS_CONTROL_HANDSHAKE\n";
        dcb.fRtsControl = RTS_CONTROL_HANDSHAKE ;

	  }  else {

        if (_notify) cout << "tpSerialLine::configLine: RTS_CONTROL_ENABLE\n";
        dcb.fRtsControl = RTS_CONTROL_ENABLE ;

	  }


      bSet = (BYTE) ((_flowcontrol & FC_XONXOFF) != 0);

	  if (bSet) if (_notify) cout << "tpSerialLine::configLine: XON_XOFF enabled\n";


      dcb.fInX = dcb.fOutX = bSet ;
      dcb.XonChar = ASCII_XON ;
      dcb.XoffChar = ASCII_XOFF ;
      dcb.XonLim = 100 ;
      dcb.XoffLim = 100 ;

*/

	    dcb.ByteSize	= 8;
		dcb.Parity	= NOPARITY;
		dcb.StopBits	= ONESTOPBIT;

		dcb.fOutxDsrFlow = 0;
		dcb.fDtrControl = DTR_CONTROL_ENABLE ;
		dcb.fOutxCtsFlow = 0;
		dcb.fRtsControl = RTS_CONTROL_ENABLE ;
		
		dcb.fInX = dcb.fOutX = 0;
		dcb.XonLim = 100;
		dcb.XoffLim = 100;


		dcb.fBinary = TRUE;
		// dcb.fParity = TRUE;

        // FOB?
        dcb.fParity = FALSE;

		// FOB ???
		// dcb.fRtsControl = RTS_CONTROL_DISABLE ;




	  if (!SetCommState(m_fd, &dcb)) {

		    tpLogError("tpSerialLine::configLine: could not change configuration on [%s]",
                m_ttyname.c_str());

			err = GetLastError();
			return 0;
	  }

      tpLogMessage("tpSerialLine::configLine: changed configuration for [%s]",
          m_ttyname.c_str());

	  	/* get any early notifications*/
     SetCommMask( m_fd, EV_RXCHAR ) ;
    /* setup device buffers*/
	// normally switched of ... for spacemouse we need 1 byte
    // SetupComm( _fd, 4096, 4096 ) ; 
    /*MIKE orig:	SetupComm( COMDEV( serial ), 4096,4096 ) ;*/
	 if ((m_use_inputbuffer > 0) && (m_use_outputbuffer > 0)) {

         tpLogMessage("tpSerialLine::configureLine: using %d input and %d outputbuffer",
             m_use_inputbuffer,m_use_outputbuffer);

		SetupComm( m_fd, m_use_inputbuffer, m_use_outputbuffer ) ;
	 }
    /* purge any information in the buffer*/
    PurgeComm( m_fd, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR ) ;





    /* set up for overlapped I/O*/
    //CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF ;

	// For SpaceMouse Set
    // CommTimeOuts.ReadIntervalTimeout = 1000;
    CommTimeOuts.ReadTotalTimeoutMultiplier = 0 ;
    CommTimeOuts.ReadTotalTimeoutConstant = 2000;

    /*normally: CommTimeOuts.ReadTotalTimeoutConstant = 1000 ;*/
    CommTimeOuts.WriteTotalTimeoutMultiplier = 0 ;
    CommTimeOuts.WriteTotalTimeoutConstant = 2000 ;
    /* normally: CommTimeOuts.WriteTotalTimeoutConstant = 1000 ;*/

	// this is devicespecific ... 
	// Fastrak : no timeout-setting
	// SpaceOrb: TimeOut Setting On (100)
	// SpaceMouse: TimeOut Setting On (???)
    if (m_use_timeout) SetCommTimeouts( m_fd, &CommTimeOuts ) ;


  }

  

  return 1;



#endif 
}



int tpRS232::convert2Baud(int int_baud) {
    int baud;
    
    switch (int_baud) {
    case 9600:   baud = B9600;  break;
    case 19200:  baud = B19200; break;
    case 38400:  baud = B38400; break;
    default:     
        baud = B9600;
        tpLogError("tpSerialLine::convert2Baud: could not convert %d baud using default 9600 baud",
            int_baud);
        break;
      
              
    }
    
    return baud;
  }
