/*
 *  Communications Node
 *  @author Andrei Polzounov
 */
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include <boost/bind.hpp>

#include "CommNode.h"
#include "CommNodeList.h"
#include "CommNodeUi.h"
#include "SyncTcpNodeCommsClient.h"
#include "UtilityFunctions.h"

namespace
{
  // Check the server every x seconds
  const int TIMER_UPDATE_SECONDS = 3;
}

using boost::asio::ip::tcp;

SyncTcpNodeCommsClient::SyncTcpNodeCommsClient(boost::asio::io_service& ioService,
    CommNodeList& sharedNodeList, const CommNodeUi& ui)
: ioService_(ioService)
, timer_(ioService_, boost::posix_time::seconds(TIMER_UPDATE_SECONDS))
, sharedNodeList_(sharedNodeList)
, ui_(ui)
{
  timer_.async_wait(boost::bind(&SyncTcpNodeCommsClient::handleTimeOutAndRestartTimer,
      this, boost::asio::placeholders::error));
}

void SyncTcpNodeCommsClient::handleTimeOutAndRestartTimer(const boost::system::error_code& error)
{
  if(!error)
  {
    readFromOtherCommNodes();
  }
  else
  {
    std::cout << error.message() << std::endl;
  }
  timer_.expires_from_now(boost::posix_time::seconds(TIMER_UPDATE_SECONDS));
  timer_.async_wait(boost::bind(&SyncTcpNodeCommsClient::handleTimeOutAndRestartTimer,
      this, boost::asio::placeholders::error));
}

void SyncTcpNodeCommsClient::readFromOtherCommNodes()
{
  // Get all the nodes that have been detected so far
  std::vector<CommNode> nodes = sharedNodeList_.list();
  int counter = 0;
  if(nodes.size() == 0)
  {
    return;
  }

  for(auto itr = nodes.begin(), end = nodes.end(); itr != end; ++itr, ++counter)
  {
    itr->isAlive = false;
    tcp::resolver resolver(ioService_);
    tcp::resolver::query query(itr->tcpServerAddress.to_string(), std::to_string(itr->tcpServerPort));
    tcp::resolver::iterator endpointIterator = resolver.resolve(query);

    boost::system::error_code error;
    int64_t timeBefore = UtilityFunctions::microsecondsSinceEpoch(); // Time set!
    tcp::socket socket(ioService_);
    try
    {
      boost::asio::connect(socket, endpointIterator);
    }
    catch (std::exception& e)
    {
      socket.close();
      // Node offline  - mark as dead and remove from the node list
      sharedNodeList_.removeCommNode(itr->sessionId);
      continue;
    }

    size_t bytesRead = socket.read_some(boost::asio::buffer(dataBuffer_), error);

    // Mark!
    int64_t timeAfter = UtilityFunctions::microsecondsSinceEpoch();
    int64_t roundTripTime = timeAfter - timeBefore;

    if(error == boost::asio::error::eof)
    {
      continue; // Connection closed cleanly by peer
    }
    else if (error)
    {
      std::cerr << error.message() << std::endl;
      throw boost::system::system_error(error); // Some other error
    }
    // Decode the TCP message
    int64_t msgTimestamp = timestampFromString(std::string(dataBuffer_, bytesRead));

    itr->isAlive = true;
    itr->roundTripTime = roundTripTime;
    itr->toOtherTripTime =  msgTimestamp - timeBefore;
    itr->fromOtherTripTime = itr->roundTripTime - itr->toOtherTripTime;
  }

  // Remove dead nodes with this one simple trick!
  nodes.erase(std::remove_if(nodes.begin(), nodes.end(),
      [](CommNode& n){ return !n.isAlive; }), nodes.end());
  ui_.updateScreen(nodes);
}

int64_t SyncTcpNodeCommsClient::timestampFromString(const std::string& string) const
{
  std::istringstream ss(string);
  int64_t timestamp;
  ss >> timestamp;
  return timestamp;
}
