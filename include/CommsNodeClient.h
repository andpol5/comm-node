#pragma once
/*
 *  Communications Node
 *  @author Andrei Polzounov
 */
#include <map>
#include <string>

#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

class CommNodeMap;

class CommsNodeClient
{
public:
  CommsNodeClient(boost::asio::io_service& ioService,
      const CommNodeMap& nodeMap);

private:
  void handleTimeOutAndRestartTimer(const boost::system::error_code& error);

  void readFromServers();

  boost::asio::io_service& ioService_;
  boost::asio::deadline_timer timer_;

  const CommNodeMap& sharedCommNodeMap_;
  std::map<std::string, CommNode> commNodeMap_;

  // Buffer for receiving messages
  static const int maxLength_ = 512;
  char dataBuffer_[512];
};
