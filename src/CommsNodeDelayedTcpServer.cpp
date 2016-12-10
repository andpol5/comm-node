/*
 *  Communications Node
 *  @author Andrei Polzounov
 */
#include <chrono>
#include <iostream>
#include <string>

#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid.hpp>

#include "CommsNodeDelayedTcpServer.h"

using boost::asio::ip::tcp;

CommsNodeDelayedTcpServer::CommsNodeDelayedTcpServer(boost::asio::io_service& ioService, int portNumber)
: ioService_(ioService)
, acceptor_(ioService_, tcp::endpoint(tcp::v4(), portNumber))
, serverPortNumber_(portNumber)
{
  auto uuid = boost::uuids::random_generator()();
  uniqueId_ = to_string(uuid);
  startAcceptingConnections();

  std::cout << "Starting slow (synchronous) server on "<< serverPortNumber_
        << ". UUID: " << uniqueId_ << std::endl;
}

void CommsNodeDelayedTcpServer::startAcceptingConnections()
{
  while(true)
  {
    tcp::socket socket(ioService_);
    acceptor_.accept(socket);
    std::string message = createMessageString();
    boost::system::error_code ignoredError;

    // Wait for 42 milliseconds
    // https://en.wikipedia.org/wiki/42_(number)#The_Hitchhiker.27s_Guide_to_the_Galaxy
    sleep42Millis();
    boost::asio::write(socket, boost::asio::buffer(message), ignoredError);
  }
}

std::string CommsNodeDelayedTcpServer::createMessageString()
{
  auto now = boost::posix_time::second_clock::universal_time();
  return std::string(uniqueId_ + "::" + boost::posix_time::to_simple_string(now));
}

void CommsNodeDelayedTcpServer::sleep42Millis()
{
  boost::posix_time::milliseconds msTime(42);
  boost::this_thread::sleep(msTime);
}
