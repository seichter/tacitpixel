/*
 * Copyright (C) 1999-2011 Hartmut Seichter
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "tp/socket.h"
#include "tp/log.h"

#include <stdlib.h>
#include <string.h>


#if defined(WIN32) || defined(WINCE)
    #include <winsock.h>
#endif

#if defined(__unix) || defined(__APPLE__) || defined(__BEOS__) || defined(__HAIKU__)
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <netdb.h>
  #include <arpa/inet.h>
  #include <unistd.h>
  #include <fcntl.h>
#endif


#if defined(WINCE)
	#pragma comment( lib, "winsock.lib" )
#else
	#include <fcntl.h>
#endif

#if defined(_WIN32) || defined(__sgi)
	typedef int socklen_t;
#endif

#if defined(__unix) || defined(__APPLE__) || defined(__BEOS__) || defined(__HAIKU__)
#include <sys/time.h>
#endif

#ifdef _WIN32
static bool gs_wsock32_init = false;
#endif


// Try to work out address from string
// returns 0 if bad
struct in_addr* tpAddressToInAddr(const tpString& address)
{
    struct hostent *host;
    static struct in_addr saddr;

    // First try nnn.nnn.nnn.nnn form
    saddr.s_addr = inet_addr(address.c_str());
    if (saddr.s_addr != -1)
        return &saddr;

    host = gethostbyname(address.c_str());
    if( host )
        return (struct in_addr *) *host->h_addr_list;

    return 0;
}


static void fillAddr(const char* address,
					 unsigned short port,
					 sockaddr_in &addr)
{
	memset(&addr, 0, sizeof(addr));  // Zero out address structure
	addr.sin_family = AF_INET;       // Internet address
	hostent *host;					// Resolve name
	if ((host = gethostbyname(address)) == NULL)
	{
	}
	addr.sin_addr.s_addr = *((unsigned long *) host->h_addr_list[0]);
	addr.sin_port = htons(port);     // Assign port in network byte order
}

tpSocket::tpSocket()
	: tpReferenced()
    , mHandle(-1)
{
}


tpSocket::tpSocket(const tpSocket& socket)
	: tpReferenced()
    , mHandle(socket.mHandle)
{
}


tpSocket::~tpSocket()
{
    this->mHandle = -1;
}



static inline bool
tpSocketInit(tpSocket& socket, unsigned int port, bool blocking = true)
{

#ifdef _WIN32
	if (!gs_wsock32_init)
	{

		WSADATA m_WSAData;

		if( WSAStartup( MAKEWORD(2,0), &m_WSAData) != 0)
		{
			tpLogError("tpSocket::doInit() : could not initialize WinSock API");
			return false;
		} else gs_wsock32_init = true;

	};
#endif

    if (socket.getType() == tpTCPSocket::getTypeInfo())
	{
			tpLogNotify("Init as TCP");
            socket.setHandle((int)::socket( PF_INET, SOCK_STREAM, IPPROTO_TCP));
	} else
    if (socket.getType() == tpUDPSocket::getTypeInfo())
	{
			tpLogNotify("Init as UDP");
            socket.setHandle((int)::socket(AF_INET, SOCK_DGRAM, 0));
	} else
	{
			tpLogError("tpSocket::doInit() undeterminded socket type!");
			return false;
    }


    if( socket.getHandle() < 0)
	{
		tpLogError("tpSocket::doInit() : could not initialize socket");
		return false;
	}


	// if (!blocking);

	return true;
}




bool
tpSocket::sendRaw(const void* indata,tpSizeT datalength,tpSizeT& total)
{
    int put = 0;
    total = 0;

    const tpChar* data = (const tpChar*)indata;

    while (datalength)
    {
       put = send(mHandle, data, datalength, 0);

        if (put > 0) {

            datalength -= put;
            data += put;
            total += put;
        } else {

            if (put == 0) {
                return true;
            }

        }
    }

    return true;
}




bool
tpSocket::bind(tpUInt port)
{
    // sockaddr initialisation
    sockaddr_in sa;
    memset( &sa, 0, sizeof( sockaddr));

    sa.sin_family = AF_INET;
    sa.sin_port = htons(port);
    sa.sin_addr.s_addr = INADDR_ANY;

    if (::bind( mHandle,(sockaddr*) &sa, sizeof(sockaddr)) < 0)
	{
		tpLogError("tpSocket::doBind() : could not bind");
		return false;
    }

	return true;
}



// --------------------------------------------------------------------------------------

tpTCPSocket::tpTCPSocket(const tpTCPSocket& socket)
	: tpSocket(socket)
{
}

tpTCPSocket::tpTCPSocket(const tpString& remotehost, unsigned int remoteport)
{
    this->connect(remotehost,remoteport);
}

tpTCPSocket::tpTCPSocket()
{
}

tpTCPSocket::tpTCPSocket(unsigned int localport)
{
	this->listen(localport);
}

tpTCPSocket::~tpTCPSocket()
{
	this->close();
}

bool
tpTCPSocket::listen(unsigned int localport)
{
    tpSocketInit(*this,localport,true);
    bind(localport);
	return this->doListenImpl(5);
}

bool
tpTCPSocket::doListenImpl(int maxpending)
{
    if( ::listen( mHandle, maxpending) < 0)
	{
		tpLogError("tpTCPSocket::doListen() : could not listen");
		return false;
	}
	return true;
}


bool
tpTCPSocket::connect(const tpString& remotehost, unsigned int remoteport)
{
	if (remotehost.isEmpty())
	{
        tpLogError("tpTCPSocket::Connect() : unsufficient parameter %s:%d",remotehost.c_str(),remoteport);
		return false;
    }

    tpSocketInit(*this,remoteport);

    // sockaddr initialisation
    sockaddr_in sa;
    memset( &sa, 0, sizeof( sockaddr));

    sa.sin_family = AF_INET;
    sa.sin_port = htons(remoteport);
    sa.sin_addr.s_addr = INADDR_ANY;

	// first guess => try to resolve it as IP@
    sa.sin_addr.s_addr = inet_addr( remotehost.c_str());

    if( sa.sin_addr.s_addr == INADDR_NONE)
	{	// screwed => try to resolve it as name

		tpLogNotify("tpTCPSocket::Connect() : connect to %s:%d",remotehost.c_str(),remoteport);

		hostent *host = gethostbyname(remotehost.c_str());
		if (!host)
		{
			tpLogError("tpTCPSocket::Connect() : unable to resolve %s",remotehost.c_str());
			return false;
		} else
		{
            sa.sin_addr.s_addr = **((int **) host->h_addr_list);
			// m_sockaddr.sin_port = htons(remoteport);
		};

	};

    if( ::connect( mHandle, ( sockaddr*)&sa, sizeof( sockaddr)))
	{
		tpLogError("tpTCPSocket::Connect() : could not connect %s:%d",remotehost.c_str(),remoteport);
		return false;
	} else return true;
}


tpTCPSocket* tpTCPSocket::accept(int timeout)
{

    if (mHandle < 0)
	{
		tpLogError("tpTCPSocket::doAccept() : can not accept on an invalid socket");
		return NULL;
	};

	bool _accept = true;

	if (timeout > -1)
	{
		fd_set fds;

		timeval tv = { timeout / 1000 , timeout % 1000 };

		FD_ZERO(&fds);
        FD_SET(mHandle, &fds);
        ::select(mHandle, &fds, 0, 0, &tv);

        _accept = (FD_ISSET(mHandle, &fds)) ? false : true;

	};

	if( _accept )
	{

		int nlen = sizeof( sockaddr);

        sockaddr_in m_sockaddr;

        int fd =  (int)::accept( mHandle, ( sockaddr *)&m_sockaddr,(socklen_t*) &nlen);

        if( fd < 0) return 0;

		tpTCPSocket* socket = new tpTCPSocket;
        socket->setHandle(fd);

		return socket;

    }

    return 0;
}


int
tpTCPSocket::write(const void* data,tpSizeT datalength)
{
    tpSizeT total = 0;
    bool state = sendRaw(data,datalength,total);

	tpLogNotify("tpTCPSocket::write() : %d/%d bytes",datalength,total);

    if (state) return total;

	return -1;
}

int tpTCPSocket::read(void* data,tpSizeT datalength)
{
    return( (int)::recv( mHandle, (char*)data, datalength, 0));
}

bool
tpTCPSocket::close()
{
    int res =
#if defined(_WIN32)
	::closesocket( mHandle);
#else
    ::close(mHandle);
#endif
    return (res == 0);
}



tpString tpTCPSocket::getRemoteAddress() const
{
    // \TODO rewrite
    tpString _ret;
//	_ret.set(inet_ntoa(m_sockaddr.sin_addr));
	return _ret;
}


// --------------------------------------------------------------------------------------



tpUDPSocket::tpUDPSocket()
	: tpSocket()
{
}

tpUDPSocket::tpUDPSocket(const tpUDPSocket& socket)
	: tpSocket(socket)
	, mReceivers(socket.mReceivers)
{
}

tpUDPSocket::tpUDPSocket(unsigned int localport) :
	tpSocket()
{
	setLocalPort(localport);
}

int
tpUDPSocket::write(const void* data,unsigned int datalength)
{
	tpInt result(0);

	for (tpReceiverMap::iterator i = mReceivers.begin();
		 i != mReceivers.end();
		 ++i)
	{
		result += this->send(data,datalength,(*i));
	}

	return result;
}

int
tpUDPSocket::read(void* data,unsigned int datalength)
{
	tpString aReceiver;
	tpUInt aPort;
	tpInt result(0);

	// @todo should map or otherwise handle the information about
	// the incoming traffic
	result = this->receiveFrom(data,datalength,aReceiver,aPort);

	return result;
}

bool
tpUDPSocket::setLocalPort(unsigned int localport)
{

    if (!tpSocketInit(*this,localport)) return false;

	// Bind the socket to its port
	sockaddr_in localAddr;
	memset(&localAddr, 0, sizeof(localAddr));
	localAddr.sin_family = AF_INET;
	localAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	localAddr.sin_port = htons(localport);

    if (::bind(mHandle, (sockaddr *) &localAddr, sizeof(sockaddr_in)) < 0)
	{
		tpLogError("tpUDPSocket::setLocalPort() : could not bind to port %d",localport);
		return false;
	}

	tpLogNotify("tpUDPSocket::setLocalPort() : successfully bind to port %d",localport);
	return true;
}



// send to remote
int
tpUDPSocket::send(const void *data, unsigned int datalength,const tpPair<tpUInt,tpString>& receiver)
{

	sockaddr_in destAddr;
	fillAddr(receiver.getValue().c_str(), receiver.getKey(), destAddr);

	// Write out the whole buffer as a single message.
    return sendto(mHandle,
		(const char*) data,
		datalength, 0,
				(sockaddr*)&destAddr,
				sizeof(struct sockaddr_in));

}

// receive from remote
int
tpUDPSocket::receiveFrom(void *data, unsigned int datalength, tpString& remoteaddress, unsigned int &remoteport, int timeout)
{

	sockaddr_in clntAddr;

	int addrLen = sizeof(struct sockaddr);
	int rtn;

	struct timeval timeout_value;
    timeout_value.tv_sec = timeout / 1000;
    timeout_value.tv_usec = (timeout % 1000) * 1000;

	fd_set fds;

	FD_ZERO(&fds);
    FD_SET(mHandle, &fds);

	if (0 == ::select(1,&fds,NULL,NULL,&timeout_value))	return -2;

    rtn = ::recvfrom(mHandle, (char*)data, datalength, 0, (sockaddr *) &clntAddr,
					  (socklen_t *) &addrLen);

	remoteaddress = inet_ntoa(clntAddr.sin_addr);
	remoteport = ntohs(clntAddr.sin_port);

	return rtn;
}



// set TTL
void
tpUDPSocket::setMulticastTTL(unsigned int TTL)
{
    if (setsockopt(mHandle, IPPROTO_IP, IP_MULTICAST_TTL,
			(char *) &TTL, sizeof(TTL)) < 0)
	{
		tpLogError("tpUDPSocket::setMulticastTTL() set failed (setsockopt())");

	}
}

// multicasting
void
tpUDPSocket::joinGroup(const tpString& multicastGroup)
{
	struct ip_mreq multicastRequest;
	multicastRequest.imr_multiaddr.s_addr = inet_addr(multicastGroup.c_str());
	multicastRequest.imr_interface.s_addr = htonl(INADDR_ANY);
    if (setsockopt(mHandle, IPPROTO_IP, IP_ADD_MEMBERSHIP,
				 (char *) &multicastRequest,
				 sizeof(multicastRequest)) < 0)
	{
		tpLogError("tpUDPSocket::joinGroup() failed");
	}
}

void
tpUDPSocket::leaveGroup(const tpString& multicastGroup)
{

	struct ip_mreq multicastRequest;
	multicastRequest.imr_multiaddr.s_addr = inet_addr(multicastGroup.c_str());
	multicastRequest.imr_interface.s_addr = htonl(INADDR_ANY);

    if (setsockopt(mHandle, IPPROTO_IP, IP_DROP_MEMBERSHIP,
	   (char *) &multicastRequest, sizeof(multicastRequest)) < 0)
	{
		tpLogError("tpUDPSocket::leaveGroup() failed");
	}
}

TP_TYPE_REGISTER(tpSocket,tpReferenced,Socket);
TP_TYPE_REGISTER(tpTCPSocket,tpSocket,TCPSocket);
TP_TYPE_REGISTER(tpUDPSocket,tpSocket,UDPSocket);
