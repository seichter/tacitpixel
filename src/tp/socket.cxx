#include "tp/socket.h"
#include "tp/log.h"

#include <stdlib.h>
#include <string.h>


#if defined(WINCE)
	#pragma comment( lib, "winsock.lib" )
#else
	#include <fcntl.h>
#endif

#if defined(_WIN32) || defined(__sgi)
	typedef int socklen_t;
#endif

#if defined(__unix) || defined(__APPLE__)
#include <sys/time.h>
#endif

#ifdef _WIN32
static bool gs_wsock32_init = false;
#endif


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
};

tpSocket::tpSocket()
    : tpReferenced(), m_socketdesc(-1)
{
}


tpSocket::tpSocket(const tpSocket& socket)
    : tpReferenced() ,m_socketdesc(socket.m_socketdesc)
{
}


tpSocket::~tpSocket()
{
	this->m_socketdesc = -1;
}



bool tpSocket::doInit(unsigned int port,bool blocking)
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

	if (this->getType() == tpTCPSocket::getTypeInfo())
	{
			tpLogNotify("Init as TCP");
			m_socketdesc = (int)::socket( PF_INET, SOCK_STREAM, IPPROTO_TCP);
	} else
	if (this->getType() == tpUDPSocket::getTypeInfo())
	{
			tpLogNotify("Init as UDP");
			m_socketdesc =  (int)::socket(AF_INET, SOCK_DGRAM, 0);
	} else
	{
			tpLogError("tpSocket::doInit() undeterminded socket type!");
			return false;
	};


	if( m_socketdesc == INVALID_SOCKET)
	{
		tpLogError("tpSocket::doInit() : could not initialize socket");
		return false;
	}

	// sockaddr initialisation
	memset( &m_sockaddr, 0, sizeof( sockaddr));

	m_sockaddr.sin_family = AF_INET;
	m_sockaddr.sin_port = htons( port);
	m_sockaddr.sin_addr.s_addr = INADDR_ANY;

	// if (!blocking);

	return true;
}




tpSocketState tpSocket::sendRaw(tpSocket* socket,void* indata,tpUInt datalength,tpUInt *total)
{
	int put = 0;
	*total = 0;

	tpChar* data = (tpChar*)indata;
	while (datalength > 0)
	{
		// if (tm_timedout(sock, TM_SEND)) return TP_SOCKTIMEOUT;

		put = send(socket->m_socketdesc, data, datalength, 0);
		if (put <= 0)
		{
#ifdef _WIN32
			if (put < 0 && WSAGetLastError() == WSAEWOULDBLOCK)
				continue;
#endif
#ifdef __CYGWIN__

			if (put < 0 && errno == EWOULDBLOCK)
				continue;
#endif
			return TP_SOCKCLOSE;
		};

		datalength -= put;
		data += put;
		*total += put;
	}
	return TP_SOCKOK;
};


bool tpSocket::bind()
{
	if (::bind( m_socketdesc,
		(sockaddr*) &m_sockaddr, sizeof(sockaddr)) < 0)
	{
		tpLogError("tpSocket::doBind() : could not bind");
		return false;
	};
	return true;
};



// --------------------------------------------------------------------------------------

tpTCPSocket::tpTCPSocket(const tpTCPSocket& socket)
	: tpSocket(socket)
{
};


tpTCPSocket::tpTCPSocket()
{
};



tpTCPSocket::tpTCPSocket(unsigned int localport)
{
	this->listen(localport);
};



tpTCPSocket::~tpTCPSocket()
{
	this->close();
};



bool tpTCPSocket::listen(unsigned int localport)
{
	doInit(localport);
	bind();
	return this->doListenImpl(5);
};



bool tpTCPSocket::doListenImpl(int maxpending)
{

	if( ::listen( m_socketdesc, maxpending) < 0)
	{
		tpLogError("tpTCPSocket::doListen() : could not listen");
		return false;
	}
	return true;
};



tpTCPSocket::tpTCPSocket(const tpString& remotehost, unsigned int remoteport)
{
	this->connect(remotehost,remoteport);
};


bool tpTCPSocket::connect(const tpString& remotehost, unsigned int remoteport)
{

	if (remotehost.isEmpty())
	{
		tpLogError("tpTCPSocket::Connect() : unsufficient parameter");
		return false;
	};

	doInit(remoteport);

	// first guess => try to resolve it as IP@
	m_sockaddr.sin_addr.s_addr = inet_addr( remotehost.c_str());

	if( m_sockaddr.sin_addr.s_addr == INADDR_NONE)
	{	// screwed => try to resolve it as name

		tpLogNotify("tpTCPSocket::Connect() : connect to %s:%d",remotehost.c_str(),remoteport);

		hostent *host = gethostbyname(remotehost.c_str());
		if (!host)
		{
			tpLogError("tpTCPSocket::Connect() : unable to resolve %s",remotehost.c_str());
			return false;
		} else
		{
            m_sockaddr.sin_addr.s_addr = **((int **) host->h_addr_list);
			// m_sockaddr.sin_port = htons(remoteport);
		};

	};

	if( ::connect( m_socketdesc, ( sockaddr*)&m_sockaddr, sizeof( sockaddr)))
	{
		tpLogError("tpTCPSocket::Connect() : could not connect %s:%d",remotehost.c_str(),remoteport);
		return false;
	} else return true;
};


tpTCPSocket* tpTCPSocket::accept(int timeout)
{

	if (m_socketdesc == INVALID_SOCKET)
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
		FD_SET(m_socketdesc, &fds);
		::select(m_socketdesc, &fds, 0, 0, &tv);

		_accept = (FD_ISSET(m_socketdesc, &fds)) ? false : true;

	};

	if( _accept )
	{

		int nlen = sizeof( sockaddr);

		int fd =  (int)::accept( m_socketdesc, ( sockaddr *)&m_sockaddr,(socklen_t*) &nlen);

		if( fd == INVALID_SOCKET) return NULL;

		tpTCPSocket* socket = new tpTCPSocket;
		socket->m_socketdesc = fd;

		return socket;

	};


	return NULL;



};


int tpTCPSocket::write(void* data,unsigned int datalength)
{
	tpUInt total = 0;
	tpSocketState state;



	state = tpSocket::sendRaw(this,data,datalength,&total);

	tpLogNotify("tpTCPSocket::write() : %d/%d bytes",datalength,total);

	if (state == TP_SOCKOK) return total;

	return -1;

	// return( (int)send( m_socketdesc, (char*)data, datalength, 0));
};

int tpTCPSocket::read(void* data,unsigned int datalength)
{
	return( (int)recv( m_socketdesc, (char*)data, datalength, 0));
};

void tpTCPSocket::close()
{
#ifdef _WIN32
	::closesocket( m_socketdesc);
#endif
#if defined(__unix) || defined(__APPLE__)
	::close(m_socketdesc);
#endif
}



tpString tpTCPSocket::getRemoteAddress() const
{

	tpString _ret;
	_ret.set(inet_ntoa(m_sockaddr.sin_addr));
	return _ret;

};


// --------------------------------------------------------------------------------------



tpUDPSocket::tpUDPSocket()
	: tpSocket(),
	m_remoteaddress("localhost"),
	m_remoteport(0)
{
};

tpUDPSocket::tpUDPSocket(const tpUDPSocket& socket)
	: tpSocket(socket)
{
	m_remoteaddress = socket.m_remoteaddress;
	m_remoteport = socket.m_remoteport;
};

tpUDPSocket::tpUDPSocket(unsigned int localport) :
	tpSocket(),
	m_remoteaddress(""),
	m_remoteport(0)
{

	setLocalPort(localport);
};



void tpUDPSocket::setBroadcast(const tpString& remoteaddress,unsigned int remoteport)
{

	m_remoteaddress = remoteaddress;
	m_remoteport = remoteport;
};


int tpUDPSocket::write(void* data,unsigned int datalength)
{
	return this->sendTo(data,datalength,m_remoteaddress.c_str(),m_remoteport);
};


int tpUDPSocket::read(void* data,unsigned int datalength)
{
	return this->receiveFrom(data,datalength,m_remoteaddress,m_remoteport);
};



bool tpUDPSocket::setLocalPort(unsigned int localport)
{

	if (!doInit(localport)) return false;

	// Bind the socket to its port
	sockaddr_in localAddr;
	memset(&localAddr, 0, sizeof(localAddr));
	localAddr.sin_family = AF_INET;
	localAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	localAddr.sin_port = htons(localport);

	if (::bind(m_socketdesc, (sockaddr *) &localAddr, sizeof(sockaddr_in)) < 0)
	{
		tpLogError("tpUDPSocket::setLocalPort() : could not bind to port %d",localport);
		return false;
	};
	tpLogNotify("tpUDPSocket::setLocalPort() : successfully bind to port %d",localport);
	return true;
};



// send to remote
int tpUDPSocket::sendTo(void *data, unsigned int datalength,
		const char* remoteaddress, unsigned int remoteport)
{

	sockaddr_in destAddr;
	fillAddr(remoteaddress, remoteport, destAddr);

	// Write out the whole buffer as a single message.
	return sendto(m_socketdesc,
		(const char*) data,
		datalength, 0,
             	(sockaddr*)&destAddr,
             	sizeof(struct sockaddr_in));

};

// receive from remote
int tpUDPSocket::receiveFrom(void *data, unsigned int datalength,
	tpString& remoteaddress, unsigned int &remoteport)
{

	sockaddr_in clntAddr;

	int addrLen = sizeof(struct sockaddr);
	int rtn;

	struct timeval timeout_value;

	timeout_value.tv_sec = m_timeout / 1000;
	timeout_value.tv_usec = (m_timeout % 1000) * 1000;

	fd_set fds;

	FD_ZERO(&fds);
	FD_SET(m_socketdesc, &fds);

	if (0 == ::select(1,&fds,NULL,NULL,&timeout_value))	return -2;

	rtn = ::recvfrom(m_socketdesc, (char*)data, datalength, 0, (sockaddr *) &clntAddr,
                      (socklen_t *) &addrLen);

	remoteaddress = inet_ntoa(clntAddr.sin_addr);
	remoteport = ntohs(clntAddr.sin_port);

	return rtn;
};




// set TTL
void tpUDPSocket::setMulticastTTL(unsigned int TTL)
{
	if (setsockopt(m_socketdesc, IPPROTO_IP, IP_MULTICAST_TTL,
			(char *) &TTL, sizeof(TTL)) < 0)
	{
		tpLogError("tpUDPSocket::setMulticastTTL() set failed (setsockopt())");

	};
};

// multicasting
void tpUDPSocket::joinGroup(const tpString& multicastGroup)
{
	struct ip_mreq multicastRequest;
	multicastRequest.imr_multiaddr.s_addr = inet_addr(multicastGroup.c_str());
	multicastRequest.imr_interface.s_addr = htonl(INADDR_ANY);
	if (setsockopt(m_socketdesc, IPPROTO_IP, IP_ADD_MEMBERSHIP,
                 (char *) &multicastRequest,
				 sizeof(multicastRequest)) < 0)
	{
		tpLogError("tpUDPSocket::joinGroup() failed");
	}
}

void tpUDPSocket::leaveGroup(const tpString& multicastGroup)
{

	struct ip_mreq multicastRequest;
	multicastRequest.imr_multiaddr.s_addr = inet_addr(multicastGroup.c_str());
	multicastRequest.imr_interface.s_addr = htonl(INADDR_ANY);

	if (setsockopt(m_socketdesc, IPPROTO_IP, IP_DROP_MEMBERSHIP,
       (char *) &multicastRequest, sizeof(multicastRequest)) < 0)
	{
		tpLogError("tpUDPSocket::leaveGroup() failed");
	};
}

TP_TYPE_REGISTER(tpSocket,tpReferenced,Socket);
TP_TYPE_REGISTER(tpTCPSocket,tpSocket,TCPSocket);
TP_TYPE_REGISTER(tpUDPSocket,tpSocket,UDPSocket);
