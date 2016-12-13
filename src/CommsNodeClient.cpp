/*
 *  Communications Node
 *  @author Andrei Polzounov
 */
#include <iostream>
#include <string>

#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "CommNodeMap.h"
#include "CommsNodeClient.h"

namespace
{
  // Check the server every x seconds
  const int TIMER_UPDATE_SECONDS = 3;
}

using boost::asio::ip::tcp;

CommsNodeClient::CommsNodeClient(boost::asio::io_service& ioService,
    const CommNodeMap& nodeMap)
: ioService_(ioService)
, timer_(ioService_, boost::posix_time::seconds(TIMER_UPDATE_SECONDS))
, sharedCommNodeMap_(nodeMap)
{
  timer_.async_wait(boost::bind(&CommsNodeClient::handleTimeOutAndRestartTimer, this,
      boost::asio::placeholders::error));
}

void CommsNodeClient::handleTimeOutAndRestartTimer(const boost::system::error_code& error)
{
  if(!error)
  {
    readFromServers();
  }
  else
  {
    std::cout << error.message() << std::endl;
  }
  timer_.expires_from_now(boost::posix_time::seconds(TIMER_UPDATE_SECONDS));
  timer_.async_wait(boost::bind(&CommsNodeClient::handleTimeOutAndRestartTimer, this,
      boost::asio::placeholders::error));
}

void CommsNodeClient::readFromServers()
{
  // Get the current list
  std::map<std::string, CommNode> map = sharedCommNodeMap_.map();
  // It should be the same size because of the signal/slot mechanism
//  if(map.size() != commNodeMap_)
//  {
//    std::cerr << "Shared map size: " << map.size()
//        << ", does not equal cached map size: " << commNodeMap_.size() << std::endl;
//  }
  commNodeMap_ = map;
  int counter = 0;
  if(commNodeMap_.size() == 0)
  {
    return;
  }
  for(auto itr = commNodeMap_.begin(), end = commNodeMap_.end(); itr != end; ++itr, ++counter)
  {
    CommNode& current = itr->second;
    std::cout << "CommNode ("<< counter+1 <<"/"<<commNodeMap_.size() << ")"
        << ", connecting to: ip: " << current.tcpServerAddress
        << ", port: " << current.tcpServerPort << ", id: " << current.sessionId <<
        ", timestamp: " << current.timeStampFirstSeen << std::endl;

    tcp::resolver resolver(ioService_);
    tcp::resolver::query query(current.tcpServerAddress.to_string(), std::to_string(current.tcpServerPort));
    tcp::resolver::iterator endpointIterator = resolver.resolve(query);

    // Time set!
    auto timeBefore = boost::posix_time::microsec_clock::universal_time().time_of_day().total_microseconds();

    boost::system::error_code error;
    tcp::socket socket(ioService_);

    try
    {
      boost::asio::connect(socket, endpointIterator);
    }
    catch (std::exception& e)
    {
      std::cerr << e.what() << std::endl;
      socket.close();
      // Skip this CommNode for now
      continue;
    }
    // while(true)

    size_t bytesRead = socket.read_some(boost::asio::buffer(dataBuffer_), error);

    // Mark!
    auto timeAfter = boost::posix_time::microsec_clock::universal_time().time_of_day().total_microseconds();
    auto diff = timeAfter - timeBefore;

    if(error == boost::asio::error::eof)
    {
      break; // Connection closed cleanly by peer
    }
    else if (error)
    {
      std::cerr << error.message() << std::endl;
      throw boost::system::system_error(error); // Some other error
    }

    std::stringstream result;
    result << "node#" << counter << " ";
    result.write(dataBuffer_, bytesRead);
    result << " timeDiff: " << diff << std::endl;
  }
}
