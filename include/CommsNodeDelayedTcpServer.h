#pragma once
/*
 *  Communications Node
 *  @author Andrei Polzounov
 */
#include <string>

#include <boost/asio.hpp>

/*
 * This version of the TCP server
 */
class CommsNodeDelayedTcpServer
{
public:
  CommsNodeDelayedTcpServer(boost::asio::io_service& ioService, int portNumber);

private:
  void startAcceptingConnections();
  std::string createMessageString();
  void sleep42Millis();

  boost::asio::io_service& ioService_;
  boost::asio::ip::tcp::acceptor acceptor_;
  int serverPortNumber_;
  std::string uniqueId_;
};
