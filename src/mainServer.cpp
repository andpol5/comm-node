/*
 *  Communications Node
 *  @author Andrei Polzounov
 */
#include <iostream>

#include <boost/asio.hpp>

#include "AsyncTcpServer.h"

using boost::asio::ip::tcp;

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
