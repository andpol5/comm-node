#pragma once
/*
 *  Communications Node
 *  @author Andrei Polzounov
 */
#include <string>

#include <boost/asio.hpp>

class CommNodeMap;

class CommsNodeDiscoveryService
{
public:
  CommsNodeDiscoveryService(boost::asio::io_service& ioService,
      const boost::asio::ip::address& multicastListenAddress,
      CommNodeMap& commNodeMap);

  void start();

private:
  void handleReceiveFrom(const boost::system::error_code& error,
      size_t bytesReceived,
      boost::asio::ip::address senderAddress);
  void bindReceiveToSocket();

  boost::asio::io_service& ioService_;
  const boost::asio::ip::address& multicastListenAddress_;
  boost::asio::ip::udp::endpoint senderEndpoint_;
  boost::asio::ip::udp::socket socket_;

  CommNodeMap& commNodeMap_;

  // Buffer for receiving messages
  static const int maxLength_ = 512;
  char dataBuffer_[512];
};
