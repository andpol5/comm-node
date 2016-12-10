#pragma once

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

using boost::asio::ip::tcp;

class ServerTcpConnection : public boost::enable_shared_from_this<ServerTcpConnection>
{
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
		message_ = makeDaytimeString();
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

	std::string makeDaytimeString()
	{
		boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
		return std::string(boost::posix_time::to_simple_string(now) + "\n");
	}

	tcp::socket socket_;
	std::string message_;
};
