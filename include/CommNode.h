#pragma once
/*
 *  Communications Node
 *  @author Andrei Polzounov
 */
#include <string>

#include <boost/asio.hpp>

class CommNode
{
public:
  std::string sessionId;
  int tcpServerPort;
  boost::asio::ip::address tcpServerAddress;
  int64_t timeStampFirstSeen;

  // Network information
  bool isAlive;
  int64_t timeStampLastReceived;   // Timestamp from when this last machine received
  int64_t roundTripTime;           // Round trip measured by TCP client
  int64_t toOtherTripTime;         // Time from this comm node to other (relies on other machine timestamp)
  int64_t fromOtherTripTime;       // Time from other comm node to this (relies on other machine timestamp)
};
