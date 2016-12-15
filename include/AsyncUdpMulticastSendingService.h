#pragma once
/*
 *  Communications Node
 *  @author Andrei Polzounov
 */
#include <string>

#include <boost/asio.hpp>

/*
 * AsyncUdpMulticastSendingService sends UDP multicasts with information about this CommNode
 */
class AsyncUdpMulticastSendingService
{
public:
  AsyncUdpMulticastSendingService(boost::asio::io_service& ioService,
      const boost::asio::ip::address& multicastAddress,
      int multicastPort,
      const std::string& multicastMessage,
      int timeoutSeconds);

private:
  void startSendingMulticasts();

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
