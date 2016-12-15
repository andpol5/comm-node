#pragma once
/*
 *  Communications Node
 *  @author Andrei Polzounov
 */
#include <string>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
//#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

#include "UtilityFunctions.h"

class ServerTcpConnection : public boost::enable_shared_from_this<ServerTcpConnection>
{
  typedef boost::asio::ip::tcp tcp;
public:
  typedef boost::shared_ptr<ServerTcpConnection> pointer;
  static pointer create(boost::asio::io_service& ioService)
  {
    return pointer(new ServerTcpConnection(ioService));
  }

  tcp::socket& socket()
  {
    return socket_;
  }

  void start()
  {
    /************************
    // Delay response here
    boost::posix_time::milliseconds msTime(57);
    boost::this_thread::sleep(msTime);
    ************************/
    boost::asio::async_write(socket_, boost::asio::buffer(currenTimeAsString()),
        boost::bind(&ServerTcpConnection::handleWrite, shared_from_this(),
            boost::asio::placeholders::error));
  }

private:
  ServerTcpConnection(boost::asio::io_service& ioService)
  : socket_(ioService)
  {
  }

  void handleWrite(const boost::system::error_code& error)
  {
    if(error)
    {
      std::cout << error.message() << std::endl;
    }
  }

  std::string currenTimeAsString() const
  {
    std::ostringstream ss;
    ss << UtilityFunctions::microsecondsSinceEpoch();
    return ss.str();
  }

  tcp::socket socket_;
};
