#pragma once
/*
 *  Communications Node
 *  @author Andrei Polzounov
 */
#include <string>

#include <boost/asio.hpp>

class CommNodeList;

/*
 * AsyncUdpMulticastListenService listens to UDP multicasts and registers their corresponding messages
 * in the shared CommNodeMap
 */
class AsyncUdpMulticastListenService
{
public:
  AsyncUdpMulticastListenService(boost::asio::io_service& ioService,
      const boost::asio::ip::address& multicastListenAddress,
      int multicastListenPort,
      CommNodeList& nodeList);

private:
  void handleReceiveFrom(const boost::system::error_code& error,
      size_t bytesReceived,
      boost::asio::ip::address senderAddress);
  void bindReceiveToSocket();

  const boost::asio::ip::address& multicastListenAddress_;
  boost::asio::ip::udp::endpoint senderEndpoint_;
  boost::asio::ip::udp::socket socket_;

  CommNodeList& sharedNodeList_;

  // Buffer for receiving messages
  static const int MAX_BUFFER_LENGTH = 512;
  char dataBuffer_[MAX_BUFFER_LENGTH];
};
