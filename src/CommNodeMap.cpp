/*
 *  Communications Node
 *  @author Andrei Polzounov
 */
#include <algorithm>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>

#include "CommNodeMap.h"

CommNodeMap::CommNodeMap(boost::mutex& mutex)
: mutex_(mutex)
{
}

void CommNodeMap::addNode(CommNode node)
{
  boost::mutex::scoped_lock lock(mutex_);
  auto foundIterator = commNodeMap_.find(node.sessionId);
  if(foundIterator == commNodeMap_.end())
  {
    long timestamp = boost::posix_time::microsec_clock::universal_time().time_of_day().total_microseconds();
    node.timeStampFirstSeen = timestamp;
    commNodeMap_.insert(std::make_pair(node.sessionId, node));
  }
}

void CommNodeMap::removeCommNode(const std::string& commNodeSessionId)
{
  boost::mutex::scoped_lock scopedLock(mutex_);
  // See if this node is in the map
  auto foundIterator = commNodeMap_.find(commNodeSessionId);
  if(foundIterator != commNodeMap_.end())
  {
    commNodeMap_.erase(foundIterator);
  }
}

std::map<std::string, CommNode> CommNodeMap::map() const
{
  boost::mutex::scoped_lock scopedLock(this->mutex_);
  return commNodeMap_;
}
