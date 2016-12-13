/*
 *  Communications Node
 *  @author Andrei Polzounov
 */
#include <iostream>
#include <string>

#include <boost/bind.hpp>

#include "CommsNodeAsyncTcpServer.h"

namespace
{
  unsigned short NULL_PORT = 0;
}

using boost::asio::ip::tcp;

CommsNodeAsyncTcpServer::CommsNodeAsyncTcpServer(boost::asio::io_service& ioService,
    const std::string& commsNodeSessionId)
: endpoint_(tcp::v4(), NULL_PORT)
, acceptor_(ioService, endpoint_)
, commsNodeSessionId_(commsNodeSessionId)
{
  // Let asio and the OS assign and bind some port number
  // and then we can read it from local_endpoint()
  acceptor_.set_option(tcp::acceptor::reuse_address(true));
  serverPortNumber_ = acceptor_.local_endpoint().port();

  std::cout << "Starting asynchronous TCP listen server on port: " << serverPortNumber_ << std::endl;
  startAcceptingConnections();
}

int CommsNodeAsyncTcpServer::serverListenPort() const
{
  return serverPortNumber_;
}

void CommsNodeAsyncTcpServer::startAcceptingConnections()
{
  ServerTcpConnection::pointer newConnection = ServerTcpConnection::create(
      acceptor_.get_io_service());

  acceptor_.async_accept(newConnection->socket(),
      boost::bind(&CommsNodeAsyncTcpServer::handleAccept, this,
          newConnection, boost::asio::placeholders::error));
}

void CommsNodeAsyncTcpServer::handleAccept(ServerTcpConnection::pointer newConnection,
    const boost::system::error_code& error)
{
  if (!error)
  {
    newConnection->start();
  }

  startAcceptingConnections();
}
