/*
 *  Communications Node
 *  @author Andrei Polzounov
 */
#include <iostream>
#include <string>

#include <boost/bind.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid.hpp>

#include "CommsNodeAsyncTcpServer.h"

using boost::asio::ip::tcp;

CommsNodeAsyncTcpServer::CommsNodeAsyncTcpServer(boost::asio::io_service& ioService,
    unsigned short portNumber)
: acceptor_(ioService, tcp::endpoint(tcp::v4(), portNumber))
, serverPortNumber_(portNumber)
{
  auto uuid = boost::uuids::random_generator()();
  uniqueId_ = to_string(uuid);
  startAcceptingConnections();

  std::cout << "Starting server. UUID: " << uniqueId_ << std::endl;
}

void CommsNodeAsyncTcpServer::startAcceptingConnections()
{
  ServerTcpConnection::pointer newConnection = ServerTcpConnection::create(
      acceptor_.get_io_service());

  newConnection->setUniqueServerId(uniqueId_);

  acceptor_.async_accept(newConnection->socket(),
      boost::bind(&CommsNodeAsyncTcpServer::handleAccept, this, newConnection,
          boost::asio::placeholders::error));
}

void CommsNodeAsyncTcpServer::handleAccept(ServerTcpConnection::pointer newConnection,
    const boost::system::error_code& error)
{
  if (!error)
    newConnection->start();

  startAcceptingConnections();
}
