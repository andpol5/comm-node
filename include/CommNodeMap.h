#pragma once
/*
 *  Communications Node
 *  @author Andrei Polzounov
 */
#include <map>
#include <string>

#include <boost/asio.hpp>
//#include <boost/signals2.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>

class CommNode
{
public:
  std::string sessionId;
  boost::asio::ip::address tcpServerAddress;
  int tcpServerPort;
  long timeStampFirstSeen;
};

class CommNodeMap
{
public:
  CommNodeMap(boost::mutex& mutex);

  void addNode(CommNode node);
  void removeCommNode(const std::string& commNodeSessionId);
  std::map<std::string, CommNode> map() const;

  // Using the boost signal mechanism similar to Qt signals and slots
  // to signal new nodes being added
//  boost::signals2::connection connect(const Signal::slot_type &subscriber)
//  {
//    return newNodeAddedSignal_.connect(subscriber);
//  }

private:
  boost::mutex& mutex_;
  std::map<std::string, CommNode> commNodeMap_;
  std::vector<CommNode> commNodeVector_;

//  Signal newNodeAddedSignal_;
};
