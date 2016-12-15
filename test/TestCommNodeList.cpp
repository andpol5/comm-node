/*
 *  Communications Node Test
 *  @author Andrei Polzounov
 */
#include <algorithm>
#include <limits>

#include <boost/generator_iterator.hpp>
#include <boost/random.hpp>
#include <gtest/gtest.h>

#include "CommNodeList.h"
#include "UtilityFunctions.h"

namespace
{
  template<typename T>
  T rand(double min=0.0, double max=1.0)
  {
    boost::mt19937 rng;
    boost::uniform_real<> dist(min, max);
    boost::variate_generator<boost::mt19937, boost::uniform_real<> > gen(rng, dist);
    return static_cast<T>(gen());
  }

  boost::asio::ip::address randIp()
  {
    uint32_t ip = rand<uint32_t>(0, std::numeric_limits<uint32_t>::max());
    unsigned char bytes[4];
    bytes[0] = ip & 0xff;
    bytes[1] = (ip >> 8) & 0xff;
    bytes[2] = (ip >> 16) & 0xff;
    bytes[3] = (ip >> 24) & 0xff;
    char buff[25];
    sprintf(&buff[0], "%d.%d.%d.%d", bytes[3], bytes[2], bytes[1], bytes[0]);
    return boost::asio::ip::address::from_string(buff);
  }

  class TestCommNodeList : public ::testing::Test
  {
  public:
    TestCommNodeList() : nodeList_(mutex_)
    {
    }

  protected:
    CommNode randomCommNode() const
    {
      CommNode n;
      n.sessionId = UtilityFunctions::generateUuid();
      n.tcpServerPort = rand<int64_t>(0, 65535);
      n.tcpServerAddress = randIp();
      n.isAlive = rand<bool>(0, 1);
      n.timeStampLastReceived = rand<int64_t>(0, UtilityFunctions::microsecondsSinceEpoch());
      n.roundTripTime = rand<int64_t>(0, UtilityFunctions::microsecondsSinceEpoch());
      n.toOtherTripTime = rand<int64_t>(0, UtilityFunctions::microsecondsSinceEpoch());
      n.fromOtherTripTime = rand<int64_t>(0, UtilityFunctions::microsecondsSinceEpoch());
      return n;
    }

    CommNode* nodeFromCurrenList(const std::string& id)
    {
      auto vector = nodeList_.list();
      auto foundIter = std::find_if(vector.begin(), vector.end(),
          [&id](const CommNode& n) {return n.sessionId == id; });
      return foundIter == vector.end() ? NULL : &(*foundIter);
    }

    boost::mutex mutex_;
    CommNodeList nodeList_;
  };
}

TEST_F(TestCommNodeList, willAddAnyNumberOfUniqueNodes)
{
  const size_t numberOfUniqueNodesToGenerate = rand<size_t>(80, 140);
  for(size_t i = 0; i < numberOfUniqueNodesToGenerate; i++)
  {
    CommNode n = randomCommNode();
    nodeList_.addNode(n);
  }
  auto vector = nodeList_.list();
  EXPECT_EQ(vector.size(), numberOfUniqueNodesToGenerate);
}

TEST_F(TestCommNodeList, willNotAddDuplicateNodes)
{
  const size_t numberOfUniqueNodesToGenerate = rand<size_t>(80, 140);
  for(size_t i = 0; i < numberOfUniqueNodesToGenerate; i++)
  {
    CommNode n = randomCommNode();
    nodeList_.addNode(n);
    nodeList_.addNode(n);
    nodeList_.addNode(n);
  }
  auto vector = nodeList_.list();
  EXPECT_EQ(vector.size(), numberOfUniqueNodesToGenerate);
}

TEST_F(TestCommNodeList, willChangeTimestampToTimeLastReceivedForOldNodes)
{
  // Prefill some nodes
  for(size_t i = 0; i < 20u; i++)
  {
    CommNode n = randomCommNode();
    nodeList_.addNode(n);
  }
  // Add another node
  CommNode n = randomCommNode();
  nodeList_.addNode(n);

  // Verify original timestamp
  CommNode* nodePtr = nodeFromCurrenList(n.sessionId);
  ASSERT_TRUE(nodePtr != NULL);
  EXPECT_EQ(nodePtr->timeStampLastReceived, n.timeStampLastReceived);

  // Increment timestamp and re-add the node
  int64_t oldTimestamp = n.timeStampLastReceived;
  n.timeStampLastReceived += rand<int>(1, 700);
  int64_t newTimestamp = n.timeStampLastReceived;
  nodeList_.addNode(n);

  // Verify new timestamp
  nodePtr = nodeFromCurrenList(n.sessionId);
  ASSERT_TRUE(nodePtr != NULL);
  EXPECT_GT(nodePtr->timeStampLastReceived, oldTimestamp);
  EXPECT_EQ(nodePtr->timeStampLastReceived, newTimestamp);
  // Everything else should be the same
  EXPECT_EQ(nodePtr->sessionId, n.sessionId);
  EXPECT_EQ(nodePtr->tcpServerAddress, n.tcpServerAddress);
  EXPECT_EQ(nodePtr->isAlive, n.isAlive);
  EXPECT_EQ(nodePtr->timeStampLastReceived, n.timeStampLastReceived);
  EXPECT_EQ(nodePtr->roundTripTime, n.roundTripTime);
  EXPECT_EQ(nodePtr->toOtherTripTime, n.toOtherTripTime);
  EXPECT_EQ(nodePtr->fromOtherTripTime, n.fromOtherTripTime);
}

TEST_F(TestCommNodeList, willSetTimestampToTimeFirstReceivedForNewNodes)
{
  int64_t maxVal = std::numeric_limits<int64_t>::max();
  for(size_t i = 0; i < 20u; i++)
  {
    CommNode n = randomCommNode();
    n.timeStampFirstSeen = maxVal;
    nodeList_.addNode(n);

    // Check that time has already been added
    CommNode* nodePtr = nodeFromCurrenList(n.sessionId);
    ASSERT_TRUE(nodePtr != NULL);
    EXPECT_NE(nodePtr->timeStampFirstSeen, maxVal);
    EXPECT_LT(nodePtr->timeStampFirstSeen, UtilityFunctions::microsecondsSinceEpoch());
  }
}
