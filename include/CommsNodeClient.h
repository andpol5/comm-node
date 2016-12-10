#pragma once
/*
 *  Communications Node
 *  @author Andrei Polzounov
 */
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

class CommsNodeClient
{
public:
  CommsNodeClient(boost::asio::io_service& ioService,
      const char* hostname, int serverPort);

private:
  void handleTimeOutAndRestartTimer();

  std::string readFromServer();

  boost::asio::io_service& ioService_;
  boost::asio::deadline_timer timer_;
  int count_;
  const char* hostname_;
  int serverPort_;
};
