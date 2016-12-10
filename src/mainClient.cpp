/*
 *  Communications Node
 *  @author Andrei Polzounov
 */
#include <iostream>

#include <boost/asio.hpp>

#include "CommsNodeClient.h"

using boost::asio::ip::tcp;

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
