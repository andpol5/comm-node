/*
 *  Communications Node
 *  @author Andrei Polzounov
 */
#include <algorithm>
#include <vector>

#include <boost/thread.hpp>

#include "CommNodeList.h"
#include "UtilityFunctions.h"

CommNodeList::CommNodeList(boost::mutex& mutex)
: mutex_(mutex)
{
}

void CommNodeList::addNode(CommNode& node)
{
  boost::mutex::scoped_lock lock(mutex_);
  auto foundIterator = commNodeMap_.find(node.sessionId);
  if(foundIterator == commNodeMap_.end())
  {
    node.timeStampFirstSeen = UtilityFunctions::microsecondsSinceEpoch();
    commNodeMap_.insert(std::make_pair(node.sessionId, node));
  }
  else
  {
    // Last received is always being updated
    foundIterator->second.timeStampLastReceived = node.timeStampLastReceived;
    // Sometimes the address is not resolved right away
    foundIterator->second.tcpServerAddress = node.tcpServerAddress;
  }
}

void CommNodeList::removeCommNode(const std::string& commNodeSessionId)
{
  boost::mutex::scoped_lock scopedLock(mutex_);
  // See if this node is in the map
  auto foundIterator = commNodeMap_.find(commNodeSessionId);
  if(foundIterator != commNodeMap_.end())
  {
    commNodeMap_.erase(foundIterator);
  }
}

std::vector<CommNode> CommNodeList::list() const
{
  boost::mutex::scoped_lock scopedLock(mutex_);

  std::vector<CommNode> vector;
  vector.resize(commNodeMap_.size());
  auto vecItr = vector.begin();
  for(auto itr = commNodeMap_.begin(), end = commNodeMap_.end(); itr != end; ++itr, ++vecItr)
  {
    *vecItr = itr->second;
  }
  return vector;
}
