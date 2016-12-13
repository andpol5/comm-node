#pragma once
/*
 *  Communications Node
 *  @author Andrei Polzounov
 */
#include <string>

class UdpMulticastMessage
{
public:
  UdpMulticastMessage();

  std::string sessionId() const;
  void setSessionId(const std::string& sessionId);

  int tcpServerPort() const;
  void setTcpServerPort(int tcpServerPort);

  // Encode/Decode to/from JSON format
  std::string encodeMessage() const;
  void decodeMessage(const std::string& data, bool& messageDecoded);

private:
  std::string sessionId_;
  int tcpServerPort_;
};
