#include <iostream>
#include <string>

#include <boost/bind.hpp>

#include "AsyncTcpServer.h"

AsyncTcpServer::AsyncTcpServer(boost::asio::io_service& ioService, unsigned short portNumber)
: acceptor_(ioService, tcp::endpoint(tcp::v4(), portNumber))
, serverPortNumber_(portNumber)
{
	startAcceptingConnections();
}

void AsyncTcpServer::startAcceptingConnections()
{
	ServerTcpConnection::pointer newConnection = ServerTcpConnection::create(
			acceptor_.get_io_service());

	acceptor_.async_accept(newConnection->socket(),
			boost::bind(&AsyncTcpServer::handleAccept, this, newConnection,
					boost::asio::placeholders::error));
}

void AsyncTcpServer::handleAccept(ServerTcpConnection::pointer newConnection,
		const boost::system::error_code& error)
{
	if (!error)
		newConnection->start();

	startAcceptingConnections();
}
