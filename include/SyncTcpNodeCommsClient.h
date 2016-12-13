#pragma once
/*
 *  Communications Node
 *  @author Andrei Polzounov
 */
#include <map>
#include <string>

#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

class CommNodeList;
class CommNodeUi;

/*
 * This class is responsible for pinging all the other detected CommNodes in the
 * directory - it is also responsible for maintaining alive/dead status and latency
 */
class SyncTcpNodeCommsClient
{
public:
  SyncTcpNodeCommsClient(boost::asio::io_service& ioService,
      CommNodeList& sharedNodeList,
      const CommNodeUi& ui);

private:
  void handleTimeOutAndRestartTimer(const boost::system::error_code& error);

  void readFromServers();

  boost::asio::io_service& ioService_;
  boost::asio::deadline_timer timer_;

  CommNodeList& sharedNodeList_;

  // Buffer for receiving TCP messages (8 bytes for 64-bit int)
  std::vector<unsigned char> dataBuffer_;
  const CommNodeUi& ui_;
};
