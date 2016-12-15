#pragma once
/*
 *  Communications Node
 *  @author Andrei Polzounov
 */
#include <map>
#include <string>

#include <boost/asio.hpp>

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
      const CommNodeUi& ui,
      int timeoutSeconds);

private:
  void handleTimeOutAndRestartTimer(const boost::system::error_code& error);

  void readFromOtherCommNodes();
  int64_t timestampFromString(const std::string& string) const;

  boost::asio::io_service& ioService_;
  boost::asio::deadline_timer timer_;

  CommNodeList& sharedNodeList_;
  const CommNodeUi& ui_;
  int timeoutSeconds_;

  // Buffer for receiving messages
  static const int MAX_BUFFER_LENGTH = 512;
  char dataBuffer_[MAX_BUFFER_LENGTH];
};
