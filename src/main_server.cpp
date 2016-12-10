#include <iostream>
#include <string>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

#include "AsyncTcpServer.h"

using boost::asio::ip::tcp;

namespace
{
	const int SERVER_PORT = 5555;

	std::string makeDaytimeString()
	{
		boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
		return std::string(boost::posix_time::to_simple_string(now) + "\n");
	}
}

int main(int argc, char* argv[])
{
	try
	{
		if (argc != 2)
		{
			std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
			return 1;
		}

		boost::asio::io_service ioService;
		AsyncTcpServer server(ioService, atoi(argv[1]));
		ioService.run();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
