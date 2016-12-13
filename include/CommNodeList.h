#pragma once
/*
 *  Communications Node
 *  @author Andrei Polzounov
 */
#include <map>

#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>

#include "CommNode.h"

class CommNode;

class CommNodeList
{
public:
  CommNodeList(boost::mutex& mutex);

  void addNode(CommNode& node);
  void removeCommNode(const std::string& commNodeSessionId);
  std::vector<CommNode> list() const;

private:
  boost::mutex& mutex_;
  std::map<std::string, CommNode> commNodeMap_;
};
