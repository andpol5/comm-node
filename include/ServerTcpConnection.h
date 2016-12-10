#pragma once
/*
 *  Communications Node
 *  @author Andrei Polzounov
 */

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

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

  void setUniqueServerId(std::string uniqueId)
  {
    uniqueId_= uniqueId;
  }

  void start()
  {
    message_ = createMessageString();
    boost::asio::async_write(socket_, boost::asio::buffer(message_),
        boost::bind(&ServerTcpConnection::handleWrite, shared_from_this(),
            boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
  }

private:
  ServerTcpConnection(boost::asio::io_service& ioService)
  :	socket_(ioService)
  {
  }

  void handleWrite(const boost::system::error_code& /*error*/, size_t /*bytes_transferred*/)
  {
  }

  std::string createMessageString()
  {
    boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
    return std::string(uniqueId_ + " " + boost::posix_time::to_simple_string(now));
  }

  tcp::socket socket_;
  std::string uniqueId_;
  std::string message_;
};
