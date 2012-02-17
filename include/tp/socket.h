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

#ifndef TPSOCKET_H
#define TPSOCKET_H

#include <tp/globals.h>
#include <tp/types.h>
#include <tp/referenced.h>
#include <tp/string.h>
#include <tp/io.h>
#include <tp/map.h>

/**
 * A simple wrapper for network sockets
 */
class TP_API tpSocket : public tpReferenced {
public:

    TP_TYPE_DECLARE

	enum {
		kTCP = 0x0,
		kUDP
	};

	//! default c'tor
	tpSocket();
	//! copy c'tor
	tpSocket(const tpSocket&);


	//! write to socket
	virtual int write(const void* data,unsigned int datalength) = 0;

	//! read from socket
	virtual int read(void* data,unsigned int datalength) = 0;

    void setHandle(int handle) { mHandle = handle; }
    int getHandle() const { return mHandle; }

    bool bind(tpUInt port);

    bool sendRaw(const void *indata, tpSizeT datalength, tpSizeT &total);

protected:

    //! default d'tor
	virtual ~tpSocket();

    int mHandle;
};



class TP_API tpTCPSocket : public tpSocket
{
public:

    TP_TYPE_DECLARE


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
    bool close();


	//! write to the socket
    virtual int write(const void* data,tpSizeT datalength);
	//! read from socket
    virtual int read(void* data,tpSizeT datalength);

	//! connect on remote address and port
	bool connect(const tpString& remoteaddress,unsigned int remoteport);

	//! get the remote address
	tpString getRemoteAddress() const;

protected:

	//! standard d'tor
	virtual ~tpTCPSocket();

	bool doListenImpl(int maxpending);

};


typedef tpMap<tpUInt,tpString> tpReceiverMap;

/*
	\class tpUDPSocket
	\brief wrapper for system UDP sockets
*/
class TP_API tpUDPSocket : public tpSocket {
public:

    TP_TYPE_DECLARE

    enum {
        kTTLSameHost = 0,
        kTTLSameSubNet = 1,
        kTTLSameSite = 32,
        kTTLSameRegion = 64,
        kTTLSameContinent = 128,
        kTTLUnrestriced = 255
    };


	//! standard c'tor
	tpUDPSocket();
	//! open a UDP socket on local machine
	tpUDPSocket(unsigned int localport);

	//! copy c'tor
	tpUDPSocket(const tpUDPSocket&);

	//! set the local sender port
	bool setLocalPort(unsigned int localport);



	//! write to the peer
	virtual int write(const void* data,unsigned int datalength);
	//! read from peer
	virtual int read(void* data,unsigned int datalength);


	//! send to remote address
	int send(const void *data,unsigned int datalength,const tpPair<tpUInt,tpString>& receiver);

	//! receive from remote address
    int receiveFrom(void *data,unsigned int datalength, tpString& remoteaddress, unsigned int &remoteport, int timeout = 10);

	//! set broadcast
	void setBroadcast(const tpString&,unsigned int);

	//! set TTL
	void setMulticastTTL(unsigned int TTL);

	//! join a multicast group
	void joinGroup(const tpString& multicastGroup);
	//! leave a multicast group
	void leaveGroup(const tpString& multicastGroup);

protected:

	tpReceiverMap mReceivers;

};


#endif
