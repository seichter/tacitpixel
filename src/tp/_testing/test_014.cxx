
#include <tp/arguments.h>
#include <tp/refptr.h>
#include <tp/socket.h>
#include <tp/log.h>

void client_implementation(const tpString& remote, tpUInt port)
{
	tpRefPtr<tpTCPSocket> tcp = new tpTCPSocket();

	if (tcp->connect(remote,port))
	{
		tpString httpreq("GET / HTTP/1.0\n\rhost: myplace.com\n\rconnection: Keep-Alive\r\nUser-Agent: HTTPTool/1.0\n\r\n\r");

		tcp->write(httpreq.c_str(),httpreq.getLength());

		tpArray<char> buffer; buffer.resize(4096);
		int r(0);
		while((r = tcp->read(buffer.getData(),buffer.getSize())) > 0)
		{
			tpLogNotify("read %d bytes from %s:%d",r,remote.c_str(),port);
		}

		tcp->close();
	}
}

void server_implementation(tpUInt port)
{
	tpRefPtr<tpTCPSocket> tcp = new tpTCPSocket();

	tcp->listen(port);

	tpRefPtr<tpTCPSocket> sock = tcp->accept();

	tpArray<char> buffer; buffer.resize(3000);

	int res = sock->read(buffer.getData(),buffer.getSize());

	buffer[res] = 0;

	tpLogNotify("received %d bytes '%s",res,buffer.getData());

}

int main(int argc,char* argv[])
{

	tpArguments args(&argc,argv);

	tpString remote;
	tpUInt port;

	args.get("--port",port);
	tpLogNotify("using port %d",port);

	if (args.get("--client",remote))
	{
		tpLogNotify("remote %s",remote.c_str());
	}

	if (!remote.isEmpty()) {
		client_implementation(remote,port);
	} else {
		server_implementation(port);
	}


	return 0;
}
