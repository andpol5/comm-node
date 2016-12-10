#include <iostream>

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

#include "CommsNodeClient.h"

using boost::asio::ip::tcp;

namespace
{
	const char* SERVER_PORT = "5555";
}


int main(int argc, char* argv[])
{
	try
	{
		if (argc != 3)
		{
			std::cerr << "Usage: " << argv[0] << " <host> <port>" << std::endl;
			return 1;
		}

		boost::asio::io_service ioService;

		CommsNodeClient client(ioService, argv[1], atoi(argv[2]));
		ioService.run();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
