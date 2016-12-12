/*
 *  Communications Node
 *  @author Andrei Polzounov
 */
#include <iostream>
#include <string>

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "CommNodeMap.h"
#include "CommsNodeAsyncTcpServer.h"
#include "CommsNodeClient.h"
#include "CommsNodeDiscoverableService.h"
#include "CommsNodeDiscoveryService.h"
#include "DiscoveryMulticastMessage.h"
#include "UuidGenerator.h"

using boost::asio::ip::tcp;
typedef boost::asio::ip::address IpAddress;

namespace
{
  const int MULTICAST_TIMEOUT_SECONDS = 1;
  const int TEMP_TCP_SERVER_PORT = 1337;
}

void tcpClientThread(CommNodeMap& nodeMap)
{
  try
  {
    boost::asio::io_service ioService;
    CommsNodeClient client(ioService, nodeMap);
    ioService.run();
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
}

void udpMulticastAndTcpServerThread(std::string sessionId, IpAddress multicastAddress)
{
  try
  {
    boost::asio::io_service ioService;
    CommsNodeAsyncTcpServer server(ioService, sessionId);

    // Fill out a discovery message with the session ID and the TCP-server port
    DiscoveryMulticastMessage discoveryMessage;
    discoveryMessage.setSessionId(sessionId);
    discoveryMessage.setTcpServerPort(server.serverListenPort());
    bool ok;
    std::string discoveryMessageString = discoveryMessage.encodeMessage(ok);

    if(!ok)
    {
      std::cerr << "Could not encode UDP-multicast message with the following params:"
                << "\n    SessionId: " << sessionId
                << "\n    TcpServerPort: " << discoveryMessage.tcpServerPort() << std::endl;
      exit(1);
    }

    CommsNodeDiscoverableService multicastService(
        ioService, multicastAddress, discoveryMessageString, MULTICAST_TIMEOUT_SECONDS);
    ioService.run();
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
}

void udpMulticastReceiverThread(IpAddress multicastListenAddress,
    CommNodeMap& nodeMap)
{
  try
  {
    boost::asio::io_service ioService;
    CommsNodeDiscoveryService discoverySvc(ioService,
        multicastListenAddress, nodeMap);
    ioService.run();
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
}

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << " <multicast_address>\n"
              << "  For IPv4, try:\n"
              << "    " << argv[0] << " 239.255.0.1\n"
              << "  For IPv6, try:\n"
              << "    " << argv[0] << " ff31::8000:1234" << std::endl;
    return 1;
  }

  IpAddress multicastAddress = IpAddress::from_string(argv[1]);

  // Generate a unique session id
  std::string uniqueSesssionId = UuidGenerator::generate();
  std::cout << "CommsNode Session id: " << uniqueSesssionId << std::endl;

  // Thread safe list of detected comm nodes
  boost::mutex nodeMapMutex;
  CommNodeMap nodeMap(nodeMapMutex);

  // Using boost::thread for Windows compatibility as the MinGW compiler
  // does not yet support std::thread
  boost::thread thread1(&udpMulticastAndTcpServerThread, uniqueSesssionId, multicastAddress);
  // Pass the nodeList by reference since boost passes by value by default
  boost::thread thread2(&udpMulticastReceiverThread, multicastAddress, boost::ref(nodeMap));
  boost::thread thread3(&tcpClientThread, boost::ref(nodeMap));

  thread1.join();
  thread2.join();
  thread3.join();

  return 0;
}
