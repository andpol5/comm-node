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
  const int TIMER_UPDATE_SECONDS = 2;
}

using boost::asio::ip::tcp;

CommsNodeClient::CommsNodeClient(boost::asio::io_service& ioService,
    const CommNodeMap& nodeMap)
: ioService_(ioService)
, timer_(ioService_, boost::posix_time::seconds(TIMER_UPDATE_SECONDS))
, sharedCommNodeMap_(nodeMap)
{
  timer_.async_wait(boost::bind(&CommsNodeClient::handleTimeOutAndRestartTimer, this));
}

void CommsNodeClient::handleTimeOutAndRestartTimer()
{
  readFromServers();

  timer_.expires_from_now(boost::posix_time::seconds(TIMER_UPDATE_SECONDS));
  timer_.async_wait(boost::bind(&CommsNodeClient::handleTimeOutAndRestartTimer, this));
}

void CommsNodeClient::readFromServers()
{
  std::cout << "reading num servers: " << commNodeMap_.size() << std::endl;
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
  for(auto itr = commNodeMap_.begin(), end = commNodeMap_.end(); itr != end; ++itr, ++counter)
  {
    CommNode& current = itr->second;
    tcp::resolver resolver(ioService_);
    tcp::resolver::query query(current.tcpServerAddress.to_string(), std::to_string(current.tcpServerPort));
    tcp::resolver::iterator endpointIterator = resolver.resolve(query);

    // Time set!
    auto timeBefore = boost::posix_time::microsec_clock::universal_time().time_of_day().total_microseconds();
    tcp::socket socket(ioService_);
    boost::asio::connect(socket, endpointIterator);

    while(true)
    {
      boost::system::error_code error;
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
        throw boost::system::system_error(error); // Some other error
      }

      std::stringstream result;
      result << "node#" << counter << " ";
      result.write(dataBuffer_, bytesRead);
      result << " timeDiff: " << diff << std::endl;
    }
  }
}
