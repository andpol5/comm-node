#pragma once
/*
 *  Communications Node
 *  @author Andrei Polzounov
 */
#include <string>

#include <boost/asio.hpp>

#include "ServerTcpConnection.h"

class CommsNodeAsyncTcpServer
{
public:
  CommsNodeAsyncTcpServer(boost::asio::io_service& ioService, unsigned short portNumber);

private:
  void startAcceptingConnections();
  void handleAccept(ServerTcpConnection::pointer newConnection,
      const boost::system::error_code& error);

  boost::asio::ip::tcp::acceptor acceptor_;
  int serverPortNumber_;
  std::string uniqueId_;
};
