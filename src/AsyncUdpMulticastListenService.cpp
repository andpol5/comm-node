/*
 *  Communications Node
 *  @author Andrei Polzounov
 */
#include <iostream>
#include <string>

#include <boost/bind.hpp>

#include "AsyncUdpMulticastListenService.h"
#include "CommNode.h"
#include "CommNodeList.h"
#include "UdpMulticastMessage.h"
#include "UtilityFunctions.h"

namespace asio = boost::asio;
namespace
{
  const asio::ip::address LISTEN_ADDRESS = asio::ip::address::from_string("0.0.0.0");
  const short MULTICAST_PORT = 30001;
}

AsyncUdpMulticastListenService::AsyncUdpMulticastListenService(asio::io_service& ioService,
    const asio::ip::address& multicastListenAddress,
    CommNodeList& nodeList)
: multicastListenAddress_(multicastListenAddress)
, socket_(ioService)
, sharedNodeList_(nodeList)
{
  boost::asio::ip::udp::endpoint listenEndpoint(LISTEN_ADDRESS, MULTICAST_PORT);	
  // Create the socket so that multiple addresses may be bound to the same address
  socket_.open(listenEndpoint.protocol());
  socket_.set_option(asio::ip::udp::socket::reuse_address(true));
  socket_.bind(listenEndpoint);

  // Join the multicast group.
  socket_.set_option(asio::ip::multicast::join_group(multicastListenAddress_));

  bindReceiveToSocket();
}

void AsyncUdpMulticastListenService::handleReceiveFrom(
    const boost::system::error_code& error,
    size_t bytesReceived, asio::ip::address senderAddress)
{
  uint64_t time = UtilityFunctions::microsecondsSinceEpoch();

  if(!error)
  {
    UdpMulticastMessage msg;
    bool messageDecoded;
    msg.decodeMessage(std::string(dataBuffer_, bytesReceived), messageDecoded);

    // Ignore the first time when the senderAddres ==  multicastListenAddress_
    if(messageDecoded && senderAddress != multicastListenAddress_)
    {
      CommNode newNode;
      newNode.timeStampLastReceived = time;
      newNode.sessionId = msg.sessionId();
      newNode.tcpServerPort = msg.tcpServerPort();
      // Server address was not part of the message, so we assume it's the same as the UDP sender
      newNode.tcpServerAddress = senderAddress;

      // Add node to the shared list
      sharedNodeList_.addNode(newNode);
    }
    else if(senderAddress != multicastListenAddress_)
    {
      std::cerr << "could not decode message: " << dataBuffer_ << std::endl;
    }
  }
  else
  {
    std::cout << error.message() << std::endl;
  }
  bindReceiveToSocket();
}

void AsyncUdpMulticastListenService::bindReceiveToSocket()
{
  socket_.async_receive_from(
      asio::buffer(dataBuffer_, MAX_BUFFER_LENGTH), senderEndpoint_,
      boost::bind(&AsyncUdpMulticastListenService::handleReceiveFrom, this,
        asio::placeholders::error,
        asio::placeholders::bytes_transferred,
        senderEndpoint_.address()));
}
