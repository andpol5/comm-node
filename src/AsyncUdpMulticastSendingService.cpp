/*
 *  Communications Node
 *  @author Andrei Polzounov
 */
#include <iostream>
#include <string>

#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>

#include "AsyncUdpMulticastSendingService.h"

using boost::asio::ip::tcp;

AsyncUdpMulticastSendingService::AsyncUdpMulticastSendingService(boost::asio::io_service& ioService,
    const boost::asio::ip::address& multicastAddress,
    int multicastPort,
    const std::string& multicastMessage,
    int timeoutSeconds)
: ioService_(ioService)
, multicastAddress_(multicastAddress)
, endpoint_(multicastAddress_, multicastPort)
, socket_(ioService, endpoint_.protocol())
, multicastMessage_(multicastMessage)
, timeoutSeconds_(timeoutSeconds)
, timer_(ioService_, boost::posix_time::seconds(timeoutSeconds_))
{
  startSendingMulticasts();
}

void AsyncUdpMulticastSendingService::startSendingMulticasts()
{
  timer_.async_wait(boost::bind(&AsyncUdpMulticastSendingService::handleTimeOutAndRestartTimer,
      this, boost::asio::placeholders::error));
}

void AsyncUdpMulticastSendingService::handleTimeOutAndRestartTimer(const boost::system::error_code& error)
{
  if(!error)
  {
    socket_.async_send_to(
        boost::asio::buffer(multicastMessage_), endpoint_,
        boost::bind(&AsyncUdpMulticastSendingService::handleSendTo, this,
          boost::asio::placeholders::error));
  }
  else
  {
    std::cout << error.message() << std::endl;
  }
}

void AsyncUdpMulticastSendingService::handleSendTo(const boost::system::error_code& error)
{
  if(!error)
  {
    timer_.expires_from_now(boost::posix_time::seconds(timeoutSeconds_));
    timer_.async_wait(boost::bind(&AsyncUdpMulticastSendingService::handleTimeOutAndRestartTimer,
        this, boost::asio::placeholders::error));
  }
  else
  {
    std::cout << error.message() << std::endl;
  }
}
