/*
 *  Communications Node
 *  @author Andrei Polzounov
 */
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <iostream>
#include <string>
#include <boost/bind.hpp>

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


CommsNodeClient::CommsNodeClient(boost::asio::io_service& ioService,
    const char* hostname, int serverPort)
: ioService_(ioService)
, timer_(ioService_, boost::posix_time::seconds(1))
, count_(0)
, hostname_(hostname)
, serverPort_(serverPort)
{
  timer_.async_wait(boost::bind(&CommsNodeClient::printDaytimeOnTimer, this));
}

void CommsNodeClient::printDaytimeOnTimer()
{

  std::cout << readDaytimeFromServer();
  ++count_;

  timer_.expires_from_now(boost::posix_time::seconds(1));
  timer_.async_wait(boost::bind(&CommsNodeClient::printDaytimeOnTimer, this));
}

std::string CommsNodeClient::readDaytimeFromServer()
{
  tcp::resolver resolver(ioService_);
  // argv1 is the IP address here -
  tcp::resolver::query query(hostname_, std::to_string(serverPort_));
  tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

  tcp::socket socket(ioService_);
  boost::asio::connect(socket, endpoint_iterator);

  while(true)
  {
    boost::array<char, 128> buf;
    boost::system::error_code error;

    size_t len = socket.read_some(boost::asio::buffer(buf), error);

    if (error == boost::asio::error::eof)
      break; // Connection closed cleanly by peer.
    else if (error)
      throw boost::system::system_error(error); // Some other error.

    std::stringstream result;
    result << count_ << " ";
    result.write(buf.data(), len);
    result << std::endl;
    return result.str();
  }
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

