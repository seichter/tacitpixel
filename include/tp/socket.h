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

#ifndef TPSOCKET_H
#define TPSOCKET_H

#include <tp/globals.h>
#include <tp/types.h>
#include <tp/referenced.h>
#include <tp/string.h>
#include <tp/io.h>

#if defined(WIN32) || defined(WINCE)
	#include <winsock.h>
#endif

#if defined(__unix) || defined(__APPLE__)
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <netdb.h>
  #include <arpa/inet.h>
  #include <unistd.h>
  #include <fcntl.h>
#endif



#define TPTTL_SAMEHOST			0
#define TPTTL_SAMESUBNET		1
#define TPTTL_SAMESITE			32
#define TPTTL_SAMEREGION		64
#define TPTTL_SAMECONTINENT		128
#define TPTTL_UNRESTRICTED		255



#if defined(__unix) || defined(__APPLE__)
	#define INVALID_SOCKET -1
#endif


#define TPSOCKET_ERROR -1

enum tpSocketState
{
	TP_SOCKOK = -1,
	TP_SOCKTIMEOUT,
	TP_SOCKCLOSE
};


/*!
	\brief a generic socket to wrap UDP and TCP sockets
	\author Hartmut Seichter
*/
class TP_API tpSocket : public tpReferenced {
public:

	TP_TYPE_DECLARE;

	enum {
		kTCP = 0x0,
		kUDP
	};

	//! default c'tor
	tpSocket();
	//! copy c'tor
	tpSocket(const tpSocket&);


	//! write to socket
	virtual int write(void* data,unsigned int datalength) = 0;

	//! read from socket
	virtual int read(void* data,unsigned int datalength) = 0;

	static tpSocketState sendRaw(tpSocket* socket,
		void* indata,
		tpUInt datalength,
		tpUInt *total);

	void setTimeOut(tpUInt timeout){m_timeout = timeout;};

	tpUInt getTimeOut() const {return m_timeout;};

protected:
	//! default d'tor
	virtual ~tpSocket();

	bool doInit(unsigned int port,bool blocking = true);
	bool bind();

	sockaddr_in m_sockaddr;
	int m_socketdesc;

	tpUInt m_timeout;
};



class TP_API tpTCPSocket : public tpSocket
{
public:

	TP_TYPE_DECLARE;


	//! standard c'tor
	tpTCPSocket();
	//! open a client TCP socket on given address and port
	tpTCPSocket(const tpString& remotehost, unsigned int remoteport);

	//! open a server socket on local machine
	tpTCPSocket(unsigned int localport);

	//! copy constructor
	tpTCPSocket(const tpTCPSocket&);

	//! get a connection through waiting
	tpTCPSocket* accept(int timeout = -1);

	//! set the socket into listen mode
	bool listen(unsigned int localport);

	//! close connection
	void close();


	//! write to the socket
	virtual int write(void* data,unsigned int datalength);
	//! read from socket
	virtual int read(void* data,unsigned int datalength);

	//! connect on remote address and port
	bool connect(const tpString& remoteaddress,unsigned int remoteport);

	//! get the remote address
	tpString getRemoteAddress() const;

protected:

	//! standard d'tor
	virtual ~tpTCPSocket();

	bool doListenImpl(int maxpending);

};


/*
	\class tpUDPSocket
	\brief wrapper for system UDP sockets
*/
class TP_API tpUDPSocket : public tpSocket
{
public:

	TP_TYPE_DECLARE;


	//! standard c'tor
	tpUDPSocket();
	//! open a UDP socket on local machine
	tpUDPSocket(unsigned int localport);

	//! copy c'tor
	tpUDPSocket(const tpUDPSocket&);

	//! set the local sender port
	bool setLocalPort(unsigned int localport);



	//! write to the peer
	virtual int write(void* data,unsigned int datalength);
	//! read from peer
	virtual int read(void* data,unsigned int datalength);


	//! send to remote address
	int sendTo(void *data,unsigned int datalength,
		const char* remoteaddress, unsigned int remoteport);

	//! receive from remote address
	int receiveFrom(void *data,unsigned int datalength,
		tpString& remoteaddress, unsigned int &remoteport);

	//! set TTL
	void setMulticastTTL(unsigned int TTL);

	//! set broadcast
	void setBroadcast(const tpString&,unsigned int);


	//! join a multicast group
	void joinGroup(const tpString& multicastGroup);
	//! leave a multicast group
	void leaveGroup(const tpString& multicastGroup);

protected:

	tpString m_remoteaddress;
	tpUInt m_remoteport;

};


#endif
