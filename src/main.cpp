/*
 *  Communications Node
 *  @author Andrei Polzounov
 */
#include <iostream>
#include <string>

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "AsyncTcpListenServer.h"
#include "AsyncUdpMulticastListenService.h"
#include "AsyncUdpMulticastSendingService.h"
#include "CommNodeList.h"
#include "CommNodeUi.h"
#include "SyncTcpNodeCommsClient.h"
#include "UdpMulticastMessage.h"
#include "UtilityFunctions.h"

using boost::asio::ip::tcp;
typedef boost::asio::ip::address IpAddress;

namespace
{
  const int MULTICAST_TIMEOUT_SECONDS = 1;
}

void tcpClientAndUiThread(CommNodeList& nodeList)
{
  try
  {
    CommNodeUi ui;

    boost::asio::io_service ioService;
    SyncTcpNodeCommsClient tcpClient(ioService, nodeList, ui);
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
    AsyncTcpListenServer server(ioService, sessionId);

    // Fill out a multicast message with the session ID and the TCP-server port
    UdpMulticastMessage multicastMessage;
    multicastMessage.setSessionId(sessionId);
    multicastMessage.setTcpServerPort(server.serverListenPort());
    std::string discoveryMessageString = multicastMessage.encodeMessage();
    AsyncUdpMulticastSendingService multicastSendingService(
        ioService, multicastAddress, discoveryMessageString, MULTICAST_TIMEOUT_SECONDS);
    ioService.run();
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
}

void udpMulticastReceiverThread(IpAddress multicastListenAddress,
    CommNodeList& nodeList)
{
  try
  {
    boost::asio::io_service ioService;
    AsyncUdpMulticastListenService multicastListenService(
        ioService, multicastListenAddress, nodeList);
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
              << "  Example: " << argv[0] << " 239.255.0.1\n" << std::endl;
    return 1;
  }

  IpAddress multicastAddress = IpAddress::from_string(argv[1]);

  // Generate a unique session id
  std::string uniqueSesssionId = UtilityFunctions::generateUuid();
  std::cout << "CommsNode Session id: " << uniqueSesssionId << std::endl;

  // Thread safe list of detected comm nodes
  boost::mutex nodeListMutex;
  CommNodeList nodeList(nodeListMutex);

  // Using boost::thread for Windows compatibility as the MinGW compiler
  // does not properly support std::thread
  boost::thread thread1(&udpMulticastAndTcpServerThread, uniqueSesssionId, multicastAddress);
  // Pass the nodeList by reference since boost passes by value by default
  boost::thread thread2(&udpMulticastReceiverThread, multicastAddress, boost::ref(nodeList));
  boost::thread thread3(&tcpClientAndUiThread, boost::ref(nodeList));

  thread1.join();
  thread2.join();
  thread3.join();

  return 0;
}
