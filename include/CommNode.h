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
  uint64_t timeStampFirstSeen;

  // Network information
  bool isAlive;
  uint64_t timeStampFromTcpMessage; // Timestamp from when the other machine sent this
  uint64_t timeStampLastReceived;   // Timestamp from when this machine received
  uint64_t roundTripTime;           // Round trip measured by tcp client
  uint64_t toOtherTripTime;         // Time from this comm node to other
  uint64_t fromOtherTripTime;       // Time from other comm node to this
};
