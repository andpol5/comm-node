#pragma once
/*
 *  Communications Node
 *  @author Andrei Polzounov
 */
#include <string>

#include <boost/asio.hpp>

class DiscoveryMulticastMessage
{
public:
  DiscoveryMulticastMessage();

  std::string sessionId() const;
  void setSessionId(const std::string& sessionId);
  bool isSessionIdValid() const;

  boost::asio::ip::address tcpServerAddress() const;
  void setTcpServerAddress(const boost::asio::ip::address& address);
  void setTcpServerAddress(const std::string& address);
  bool isTcpServerAddressValid() const;

  int tcpServerPort() const;
  void setTcpServerPort(int tcpServerPort);
  bool isTcpServerPortValid() const;

  // Encode/Decode to/from JSON format
  std::string encodeMessage(bool& messageEncoded) const;
  void decodeMessage(std::string data, bool& messageDecoded);

private:
  std::string sessionId_;
  boost::asio::ip::address tcpServerAddress_;
  int tcpServerPort_;

  bool isSessionIdValid_;
  bool isTcpServerAddressValid_;
  bool isTcpServerPortValid_;
};
