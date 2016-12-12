/*
 *  Communications Node
 *  @author Andrei Polzounov
 */
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include "DiscoveryMulticastMessage.h"

namespace
{
  // According to RFC 5737 this IP Address is reserved for documentation purposes
  const std::string DEFAULT_ADDRESS = "192.0.2.0";
  const std::string DEFAULT_SESSION_ID = "InvalidSessionId";
  const int DEFAULT_PORT = -1;
}

namespace pt = boost::property_tree;

DiscoveryMulticastMessage::DiscoveryMulticastMessage()
: sessionId_(DEFAULT_SESSION_ID)
, tcpServerAddress_(boost::asio::ip::address::from_string(DEFAULT_ADDRESS))
, tcpServerPort_(DEFAULT_PORT)
, isSessionIdValid_(false)
, isTcpServerAddressValid_(false)
, isTcpServerPortValid_(false)
{
}

std::string DiscoveryMulticastMessage::sessionId() const
{
  return sessionId_;
}

void DiscoveryMulticastMessage::setSessionId(const std::string& sessionId)
{
  isSessionIdValid_= true;
  sessionId_= sessionId;
}

bool DiscoveryMulticastMessage::isSessionIdValid() const
{
  return isSessionIdValid_;
}

boost::asio::ip::address DiscoveryMulticastMessage::tcpServerAddress() const
{
  return tcpServerAddress_;
}

void DiscoveryMulticastMessage::setTcpServerAddress(const boost::asio::ip::address& address)
{
  isTcpServerAddressValid_ = true;
  tcpServerAddress_ = address;
}

void DiscoveryMulticastMessage::setTcpServerAddress(const std::string& address)
{
  isTcpServerAddressValid_ = true;
  tcpServerAddress_ = boost::asio::ip::address::from_string(address);
}

bool DiscoveryMulticastMessage::isTcpServerAddressValid() const
{
  return isTcpServerAddressValid_;
}

int DiscoveryMulticastMessage::tcpServerPort() const
{
  return tcpServerPort_;
}

void DiscoveryMulticastMessage::setTcpServerPort(int tcpServerPort)
{
  isTcpServerPortValid_ = true;
  tcpServerPort_ = tcpServerPort;
}

bool DiscoveryMulticastMessage::isTcpServerPortValid() const
{
  return isTcpServerPortValid_;
}

std::string DiscoveryMulticastMessage::encodeMessage(bool& messageEncoded) const
{
  if(!isSessionIdValid_ || !isTcpServerPortValid_)
  {
    messageEncoded = false;
    return DEFAULT_SESSION_ID;
  }
  else
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
    messageEncoded = true;
    return dataAsString;
  }
}

void DiscoveryMulticastMessage::decodeMessage(std::string data, bool& messageDecoded)
{
  std::stringstream dataAsStringStream(data);
  pt::ptree root;
  pt::read_json(dataAsStringStream, root);
  std::string sessionId = root.get<std::string>("SessionId", DEFAULT_SESSION_ID);
  int tcpServerPort = root.get<int>("TcpServerPort", DEFAULT_PORT);

  isSessionIdValid_ = (sessionId != DEFAULT_SESSION_ID);
  if(isSessionIdValid_)
  {
    setSessionId(sessionId);
  }
  isTcpServerPortValid_ = (tcpServerPort != DEFAULT_PORT);
  if(tcpServerPort != DEFAULT_PORT)
  {
    setTcpServerPort(tcpServerPort);
  }
  messageDecoded = (isSessionIdValid_ && isTcpServerPortValid_);
}
