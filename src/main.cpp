/*
 *  Communications Node
 *  @author Andrei Polzounov
 */
#include <chrono>
#include <iostream>

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "CommsNodeAsyncTcpServer.h"
#include "CommsNodeClient.h"

using boost::asio::ip::tcp;

void serverThread(int serverPortNumber)
{
  try
  {
    boost::asio::io_service ioService;
    CommsNodeAsyncTcpServer server(ioService, serverPortNumber);
    ioService.run();
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
}

void clientThread(const char* hostname, int serverPortNumber)
{
  try
  {
    boost::asio::io_service ioService;
    CommsNodeClient client(ioService, hostname, serverPortNumber);
    ioService.run();
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
}

int main(int argc, char* argv[])
{
  if (argc != 3)
  {
    std::cerr << "Usage: " << argv[0] << " <host> <port>" << std::endl;
    return 1;
  }
  int portNumber = atoi(argv[2]);

  // Using boost::thread for Windows compatibility as the MinGW compiler
  // does not yet support std::thread
  boost::thread sThread(&serverThread, portNumber);
  boost::thread cThread(&clientThread, argv[1], portNumber);

  sThread.join();
  cThread.join();

  return 0;
}
