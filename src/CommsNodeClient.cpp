/*
 *  Communications Node
 *  @author Andrei Polzounov
 */
#include <iostream>
#include <string>

#include <boost/bind.hpp>

#include "CommsNodeClient.h"

using boost::asio::ip::tcp;

CommsNodeClient::CommsNodeClient(boost::asio::io_service& ioService,
    const char* hostname, int serverPort)
: ioService_(ioService)
, timer_(ioService_, boost::posix_time::seconds(1))
, count_(0)
, hostname_(hostname)
, serverPort_(serverPort)
{
  timer_.async_wait(boost::bind(&CommsNodeClient::printDaytimeOnTimer, this));
}

void CommsNodeClient::printDaytimeOnTimer()
{

  std::cout << readDaytimeFromServer();
  ++count_;

  timer_.expires_from_now(boost::posix_time::seconds(1));
  timer_.async_wait(boost::bind(&CommsNodeClient::printDaytimeOnTimer, this));
}

std::string CommsNodeClient::readDaytimeFromServer()
{
  tcp::resolver resolver(ioService_);
  // argv1 is the IP address here -
  tcp::resolver::query query(hostname_, std::to_string(serverPort_));
  tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

  tcp::socket socket(ioService_);
  boost::asio::connect(socket, endpoint_iterator);

  while(true)
  {
    boost::array<char, 128> buf;
    boost::system::error_code error;

    size_t len = socket.read_some(boost::asio::buffer(buf), error);

    if (error == boost::asio::error::eof)
      break; // Connection closed cleanly by peer.
    else if (error)
      throw boost::system::system_error(error); // Some other error.

    std::stringstream result;
    result << count_ << " ";
    result.write(buf.data(), len);
    result << std::endl;
    return result.str();
  }
}
