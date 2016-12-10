#pragma once

#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

using boost::asio::ip::tcp;

class CommsNodeClient
{
public:
	CommsNodeClient(boost::asio::io_service& ioService,
			const char* hostname, int serverPort);

	void printDaytimeOnTimer();

private:
	std::string readDaytimeFromServer();

	boost::asio::io_service& ioService_;
	boost::asio::deadline_timer timer_;
	int count_;
	const char* hostname_;
	int serverPort_;
};
