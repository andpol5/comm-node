/*
 *  Communications Node
 *  @author Andrei Polzounov
 */
#include <iostream>
#include <limits>
#include <string>

#include <boost/asio.hpp>
#include <boost/program_options.hpp>
#include <boost/thread.hpp>



using boost::asio::ip::tcp;
typedef boost::asio::ip::address ip_address;

namespace
{
  // Default program settings
  const ip_address DEFAULT_MULTICAST_ADDRESS = ip_address::from_string("239.255.0.1");
  const int DEFAULT_TCP_CLIENT_TIMEOUT_SECONDS = 3;
  const int DEFAULT_MULTICAST_TIMEOUT_SECONDS = 1;
  const int DEFAULT_MULTICAST_PORT = 30001;
}

void tcpClientAndUiThread(std::string sessionId,
    ip_address multicastAddress, int multicastPort, int clientTimeout, CommNodeList& nodeList)
{
  try
  {
    CommNodeUi ui(sessionId, multicastAddress, multicastPort);

    boost::asio::io_service ioService;
    SyncTcpNodeCommsClient tcpClient(ioService, nodeList, ui, clientTimeout);
    ioService.run();
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
}

void udpMulticastAndTcpServerThread(std::string sessionId,
    ip_address multicastAddress, int multicastPort, int multicastTimeout)
{
  try
  {
    boost::asio::io_service ioService;
    AsyncTcpListenServer server(ioService);

    // Fill out a multicast message with the session ID and the TCP-server port
    UdpMulticastMessage multicastMessage;
    multicastMessage.setSessionId(sessionId);
    multicastMessage.setTcpServerPort(server.serverListenPort());
    std::string discoveryMessageString = multicastMessage.encodeMessage();

    AsyncUdpMulticastSendingService multicastSendingService(
        ioService, multicastAddress, multicastPort,
        discoveryMessageString, multicastTimeout);

    ioService.run();
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
}

void udpMulticastReceiverThread(ip_address multicastListenAddress, int multicastPort,
    CommNodeList& nodeList)
{
  try
  {
    boost::asio::io_service ioService;
    AsyncUdpMulticastListenService multicastListenService(
        ioService, multicastListenAddress, multicastPort, nodeList);
    ioService.run();
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
}

void getCmdProgramOptions(int argc, char** argv,
    ip_address& address, int& port, int& multicastTimeout, int& clientTimeout)
{
  namespace po = boost::program_options;
  po::options_description description("Options");

  try
  {
    description.add_options()
        ("help", "Print help message and exit")
        ("client_timeout", po::value<int>()->default_value(DEFAULT_TCP_CLIENT_TIMEOUT_SECONDS),
            "Refresh rate for displaying available comm nodes (s)")
        ("multicast_timeout", po::value<int>()->default_value(DEFAULT_MULTICAST_TIMEOUT_SECONDS),
            "Sending rate for multicast messages (s)")
        ("multicast_address", po::value<std::string>(),
            "UDP-multicast address - other nodes must join multicast group")
        ("multicast_port", po::value<short unsigned int>()->default_value(DEFAULT_MULTICAST_PORT),
            "UDP-multicast port");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, description), vm);
    po::notify(vm);

    if(vm.count("help"))
    {
      std::cout << description << std::endl;
      exit(1);
    }

    clientTimeout = vm["client_timeout"].as<int>();
    multicastTimeout = vm["multicast_timeout"].as<int>();
    port = static_cast<int>(vm["multicast_port"].as<short unsigned int>());

    // Check argument boundaries
    if(clientTimeout < 0)
    {
      std::cerr << "Client refresh rate cannot be negative" << std::endl;
      exit(1);
    }

    if(multicastTimeout < 0)
    {
      std::cerr << "Multicast sending rate cannot be negative" << std::endl;
      exit(1);
    }

    if(0 <= port && port < 1024)
    {
      std::cerr << "Port number: " << port << " falls into the reserved range 0-1024" << std::endl;
      exit(1);
    }
    else if(port < 0 || port > 65535)
    {
      std::cerr << "Port number: " << port << " is out of bounds" << std::endl;
      exit(1);
    }

    if(vm.count("multicast_address"))
    {
      std::string addressString = vm["multicast_address"].as<std::string>();
      boost::system::error_code ec;
      address = ip_address::from_string(addressString, ec);
      if(ec)
      {
        std::cerr << ec.message() << std::endl;
        exit(1);
      }
      if(!address.is_v4())
      {
        std::cerr << "Only IPv4 addresses supported" << std::endl;
        exit(1);
      }
      if(!address.to_v4().is_multicast())
      {
        std::cerr << address.to_v4().to_string() << " is not a multicast address. "
          << "Multicast range is 224.0.0.0 to 239.255.255.255" << std::endl;
        exit(1);
      }
    }
    else
    {
      address = DEFAULT_MULTICAST_ADDRESS;
    }
  }
  catch(po::error& e)
  {
    std::cerr << e.what() << std::endl << description << std::endl;
    exit(1);
  }
}

int main(int argc, char** argv)
{
  // Get command line arguments using boost::program_options
  ip_address multicastAddress;
  int multicastPort;
  int multicastTimeout;
  int clientTimeout;
  getCmdProgramOptions(argc, argv, multicastAddress, multicastPort, multicastTimeout, clientTimeout);

  std::cout << "Client refresh rate: " << clientTimeout << " seconds\n"
            << "Multicast sending period: " << multicastTimeout << " seconds\n"
            << "Multicast address: " << multicastAddress << ":"
            << multicastPort << std::endl;

  // Generate a unique session id
  std::string uniqueSesssionId = UtilityFunctions::generateUuid();
  std::cout << "CommsNode Session id: " << uniqueSesssionId << std::endl;

  // Thread safe list of detected comm nodes
  boost::mutex nodeListMutex;
  CommNodeList nodeList(nodeListMutex);

  // Using boost::thread for Windows compatibility as the MinGW compiler
  // does not properly support std::thread
  boost::thread thread1(&udpMulticastAndTcpServerThread,
      uniqueSesssionId, multicastAddress, multicastPort, multicastTimeout);
  // Pass the nodeList by reference since boost passes by value by default
  boost::thread thread2(&udpMulticastReceiverThread,
      multicastAddress, multicastPort, boost::ref(nodeList));
  boost::thread thread3(&tcpClientAndUiThread,
      uniqueSesssionId, multicastAddress, multicastPort, clientTimeout, boost::ref(nodeList));

  thread1.join();
  thread2.join();
  thread3.join();

  return 0;
}
