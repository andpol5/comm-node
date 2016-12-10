#pragma once

#include <boost/asio.hpp>

#include "ServerTcpConnection.h"

using boost::asio::ip::tcp;

class AsyncTcpServer
{
public:
	AsyncTcpServer(boost::asio::io_service& ioService, unsigned short portNumber);

private:
	void startAcceptingConnections();
	void handleAccept(ServerTcpConnection::pointer newConnection,
			const boost::system::error_code& error);

	tcp::acceptor acceptor_;
	int serverPortNumber_;
};
