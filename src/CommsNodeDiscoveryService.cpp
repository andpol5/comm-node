/*
 *  Communications Node
 *  @author Andrei Polzounov
 */
#include <iostream>
#include <string>

#include <boost/bind.hpp>
#include <boost/thread.hpp>

#include "CommNodeMap.h"
#include "CommsNodeDiscoveryService.h"
#include "DiscoveryMulticastMessage.h"

namespace
{
  const short MULTICAST_PORT = 30001;
}
namespace asio = boost::asio;

CommsNodeDiscoveryService::CommsNodeDiscoveryService(asio::io_service& ioService,
    const asio::ip::address& multicastListenAddress,
    CommNodeMap& commNodeMap)
: ioService_(ioService)
, multicastListenAddress_(multicastListenAddress)
, senderEndpoint_(multicastListenAddress_, MULTICAST_PORT)
, socket_(ioService)
, commNodeMap_(commNodeMap)
{
  start();
}

void CommsNodeDiscoveryService::start()
{
  // Create the socket so that multiple addresses may be bound to the same address.
  socket_.open(senderEndpoint_.protocol());
  socket_.set_option(asio::ip::udp::socket::reuse_address(true));
  socket_.bind(senderEndpoint_);

  // Join the multicast group.
  socket_.set_option(asio::ip::multicast::join_group(multicastListenAddress_));

  bindReceiveToSocket();
}

void CommsNodeDiscoveryService::handleReceiveFrom(
    const boost::system::error_code& error,
    size_t bytesReceived, asio::ip::address senderAddress)
{

  if(!error)
  {
    DiscoveryMulticastMessage msg;
    bool messageDecoded;
    msg.decodeMessage(std::string(dataBuffer_, bytesReceived), messageDecoded);

    // Ignore the first time when the senderAddres ==  multicastListenAddress_
    if(messageDecoded && senderAddress != multicastListenAddress_)
    {
      CommNode newNode;
      newNode.sessionId = msg.sessionId();
      newNode.tcpServerPort = msg.tcpServerPort();
      // Server address was not part of the message, so we assume it's the same as the UDP sender
      newNode.tcpServerAddress = senderAddress;

      // Add node to the shared list
      commNodeMap_.addNode(newNode);

      //std::cout << "Received nodeMsg: " << newNode.sessionId << ", " << newNode.tcpServerPort
      //    << ", " << newNode.tcpServerAddress.to_string() << std::endl;
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

void CommsNodeDiscoveryService::bindReceiveToSocket()
{
  socket_.async_receive_from(
      asio::buffer(dataBuffer_, maxLength_), senderEndpoint_,
      boost::bind(&CommsNodeDiscoveryService::handleReceiveFrom, this,
        asio::placeholders::error,
        asio::placeholders::bytes_transferred,
        senderEndpoint_.address()));
}
