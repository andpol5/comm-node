#pragma once
/*
 *  Communications Node
 *  @author Andrei Polzounov
 */
#include <string>

#include <boost/asio.hpp>

class CommsNodeDiscoverableService
{
public:
  CommsNodeDiscoverableService(boost::asio::io_service& ioService,
      const boost::asio::ip::address& multicastAddress,
      const std::string& multicastMessage,
      int timeoutSeconds);

  void start();
  void stop();

private:
  // Both of the handle functions are asynchronous
  void handleTimeOutAndRestartTimer(const boost::system::error_code& error);
  void handleSendTo(const boost::system::error_code& error);

  boost::asio::io_service& ioService_;
  boost::asio::ip::address multicastAddress_;
  boost::asio::ip::udp::endpoint endpoint_;
  boost::asio::ip::udp::socket socket_;

  const std::string& multicastMessage_;
  int timeoutSeconds_;
  boost::asio::deadline_timer timer_;
};
