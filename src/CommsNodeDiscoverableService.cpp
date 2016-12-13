/*
 *  Communications Node
 *  @author Andrei Polzounov
 */
#include <iostream>
#include <string>

#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>

#include "CommsNodeDiscoverableService.h"

namespace
{
  const short MULTICAST_PORT = 30001;
}

using boost::asio::ip::tcp;

CommsNodeDiscoverableService::CommsNodeDiscoverableService(boost::asio::io_service& ioService,
    const boost::asio::ip::address& multicastAddress, const std::string& multicastMessage,
    int timeoutSeconds)
: ioService_(ioService)
, multicastAddress_(multicastAddress)
, endpoint_(multicastAddress_, MULTICAST_PORT)
, socket_(ioService, endpoint_.protocol())
, multicastMessage_(multicastMessage)
, timeoutSeconds_(timeoutSeconds)
, timer_(ioService_, boost::posix_time::seconds(timeoutSeconds_))
{
  std::cout << "Starting UDP-multicast discovery service to IP: "
      << multicastAddress_.to_string() << std::endl;

  start();
}

void CommsNodeDiscoverableService::start()
{
  timer_.async_wait(boost::bind(&CommsNodeDiscoverableService::handleTimeOutAndRestartTimer,
      this, boost::asio::placeholders::error));
}

void CommsNodeDiscoverableService::stop()
{
  timer_.cancel();
}

void CommsNodeDiscoverableService::handleTimeOutAndRestartTimer(const boost::system::error_code& error)
{
//  std::cout << "DEBUG: UdpMulticaster - handled timeout" << std::endl;
  if(!error)
  {
    socket_.async_send_to(
        boost::asio::buffer(multicastMessage_), endpoint_,
        boost::bind(&CommsNodeDiscoverableService::handleSendTo, this,
          boost::asio::placeholders::error));
  }
  else
  {
    std::cout << error.message() << std::endl;
  }
}

void CommsNodeDiscoverableService::handleSendTo(const boost::system::error_code& error)
{
//  std::cout << "DEBUG: UdpMulticaster - handled sendto" << std::endl;
  if(!error)
  {
    timer_.expires_from_now(boost::posix_time::seconds(timeoutSeconds_));
    timer_.async_wait(boost::bind(&CommsNodeDiscoverableService::handleTimeOutAndRestartTimer,
        this, boost::asio::placeholders::error));
  }
  else
  {
    std::cout << error.message() << std::endl;
  }
}
