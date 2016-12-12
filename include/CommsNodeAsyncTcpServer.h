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
  CommsNodeAsyncTcpServer(boost::asio::io_service& ioService,
      const std::string& commsNodeSessionId);

  int serverListenPort() const;

private:
  void startAcceptingConnections();
  void handleAccept(ServerTcpConnection::pointer newConnection,
      const boost::system::error_code& error);

  boost::asio::ip::tcp::endpoint endpoint_;
  boost::asio::ip::tcp::acceptor acceptor_;
  const std::string& commsNodeSessionId_;
  int serverPortNumber_;
};
