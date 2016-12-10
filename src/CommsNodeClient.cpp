/*
 *  Communications Node
 *  @author Andrei Polzounov
 */
#include <iostream>
#include <string>

#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "CommsNodeClient.h"

namespace
{
  // Check the server every x seconds
  const int TIMER_UPDATE_SECONDS = 2;
  const int MESSAGE_BUFFER_MAX_SIZE = 128;
}

using boost::asio::ip::tcp;

CommsNodeClient::CommsNodeClient(boost::asio::io_service& ioService,
    const char* hostname, int serverPort)
: ioService_(ioService)
, timer_(ioService_, boost::posix_time::seconds(TIMER_UPDATE_SECONDS))
, count_(0)
, hostname_(hostname)
, serverPort_(serverPort)
{
  timer_.async_wait(boost::bind(&CommsNodeClient::handleTimeOutAndRestartTimer, this));
}

void CommsNodeClient::handleTimeOutAndRestartTimer()
{
  std::cout << readFromServer();
  ++count_;

  timer_.expires_from_now(boost::posix_time::seconds(TIMER_UPDATE_SECONDS));
  timer_.async_wait(boost::bind(&CommsNodeClient::handleTimeOutAndRestartTimer, this));
}

std::string CommsNodeClient::readFromServer()
{
  tcp::resolver resolver(ioService_);
  tcp::resolver::query query(hostname_, std::to_string(serverPort_));
  tcp::resolver::iterator endpointIterator = resolver.resolve(query);

  // Time set!
  auto timeBefore = boost::posix_time::microsec_clock::local_time().time_of_day().total_milliseconds();
  tcp::socket socket(ioService_);
  boost::asio::connect(socket, endpointIterator);

  while(true)
  {
    char buff[MESSAGE_BUFFER_MAX_SIZE];
    boost::system::error_code error;
    size_t len = socket.read_some(boost::asio::buffer(buff), error);

    // Mark!
    auto timeAfter = boost::posix_time::microsec_clock::local_time().time_of_day().total_milliseconds();
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
    result << count_ << " ";
    result.write(buff, len);
    result << " timeDiff: " << diff << std::endl;
    return result.str();
  }
}
