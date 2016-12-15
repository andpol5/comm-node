/*
 *  Communications Node
 *  @author Andrei Polzounov
 */
#include <iostream>

#include <boost/bind.hpp>

#include "AsyncTcpListenServer.h"

namespace
{
  unsigned short NULL_PORT = 0;
}

using boost::asio::ip::tcp;

AsyncTcpListenServer::AsyncTcpListenServer(boost::asio::io_service& ioService)
: endpoint_(tcp::v4(), NULL_PORT)
, acceptor_(ioService, endpoint_)
{
  // Let asio and the OS assign and bind some port number
  // and then we can read it from local_endpoint()
  acceptor_.set_option(tcp::acceptor::reuse_address(true));
  serverPortNumber_ = acceptor_.local_endpoint().port();

  std::cout << "Starting async TCP listen server on port: " << serverPortNumber_ << std::endl;
  startAcceptingConnections();
}

int AsyncTcpListenServer::serverListenPort() const
{
  return serverPortNumber_;
}

void AsyncTcpListenServer::startAcceptingConnections()
{
  ServerTcpConnection::pointer newConnection = ServerTcpConnection::create(
      acceptor_.get_io_service());

  acceptor_.async_accept(newConnection->socket(),
      boost::bind(&AsyncTcpListenServer::handleAccept, this,
          newConnection, boost::asio::placeholders::error));
}

void AsyncTcpListenServer::handleAccept(ServerTcpConnection::pointer newConnection,
    const boost::system::error_code& error)
{
  if (!error)
  {
    newConnection->start();
  }

  startAcceptingConnections();
}
