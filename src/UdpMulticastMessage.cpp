/*
 *  Communications Node
 *  @author Andrei Polzounov
 */
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include "UdpMulticastMessage.h"

namespace
{
  const std::string INVALID_SESSION_ID = "InvalidSessionId";
  const int INVALID_PORT = -1;
}

namespace pt = boost::property_tree;

UdpMulticastMessage::UdpMulticastMessage()
: sessionId_(INVALID_SESSION_ID)
, tcpServerPort_(INVALID_PORT)
{
}

std::string UdpMulticastMessage::sessionId() const
{
  return sessionId_;
}

void UdpMulticastMessage::setSessionId(const std::string& sessionId)
{
  sessionId_= sessionId;
}

int UdpMulticastMessage::tcpServerPort() const
{
  return tcpServerPort_;
}

void UdpMulticastMessage::setTcpServerPort(int tcpServerPort)
{
  tcpServerPort_ = tcpServerPort;
}

std::string UdpMulticastMessage::encodeMessage() const
{
  pt::ptree root;
  root.put("SessionId", sessionId_);
  root.put("TcpServerPort", tcpServerPort_);

  std::stringstream dataAsStringStream;
  pt::write_json(dataAsStringStream, root);
  std::string dataAsString = dataAsStringStream.str();
  // Strip out the whitespace that boost::property_tree added
  dataAsString.erase(std::remove_if(dataAsString.begin(), dataAsString.end(),
      ::isspace), dataAsString.end());
  return dataAsString;
}

void UdpMulticastMessage::decodeMessage(const std::string& data, bool& messageDecoded)
{
  std::stringstream dataAsStringStream(data);
  pt::ptree root;
  pt::read_json(dataAsStringStream, root);
  sessionId_ = root.get<std::string>("SessionId", INVALID_SESSION_ID);
  tcpServerPort_ = root.get<int>("TcpServerPort", INVALID_PORT);

  messageDecoded = (sessionId_ != INVALID_SESSION_ID) && (tcpServerPort_ != INVALID_PORT);
}
