/*
 *  Communications Node Test
 *  @author Andrei Polzounov
 */
#include <gtest/gtest.h>

#include "UdpMulticastMessage.h"

namespace
{
  const std::string INVALID_SESSION_ID = "InvalidSessionId";
  const int INVALID_PORT = -1;

  class TestUdpMulticastMessage : public ::testing::Test
  {
  protected:
    UdpMulticastMessage msg_;
  };
}

TEST_F(TestUdpMulticastMessage, willReturnDefaultValues)
{
  EXPECT_EQ(msg_.sessionId(), INVALID_SESSION_ID);
  EXPECT_EQ(msg_.tcpServerPort(), INVALID_PORT);
}

TEST_F(TestUdpMulticastMessage, willGetNewValuesAfterSetting)
{
  msg_.setSessionId("someId");
  msg_.setTcpServerPort(5864);

  EXPECT_EQ(msg_.sessionId(), "someId");
  EXPECT_EQ(msg_.tcpServerPort(), 5864);
}

TEST_F(TestUdpMulticastMessage, willEncodeToJson)
{
  msg_.setSessionId("2c9c0570-1c01-1010-f336cf967235");
  msg_.setTcpServerPort(1337);

  const std::string expectedJsonMessage =
        "{\"SessionId\":\"2c9c0570-1c01-1010-f336cf967235\",\"TcpServerPort\":\"1337\"}";

  std::string data = msg_.encodeMessage();
  EXPECT_EQ(data, expectedJsonMessage);
}

TEST_F(TestUdpMulticastMessage, willDecodeFromJson)
{
  // Note the message does not include ip address (the sender doesn't know its external ip)
  const std::string jsonMessage =
      "{\"SessionId\":\"7c9c0442-e1d6-4b16-81fe-f336cf967235\",\"TcpServerPort\":\"5775\"}";
  bool isDecoded;
  msg_.decodeMessage(jsonMessage, isDecoded);

  EXPECT_EQ(msg_.sessionId(), "7c9c0442-e1d6-4b16-81fe-f336cf967235");
  EXPECT_EQ(msg_.tcpServerPort(), 5775);
  EXPECT_TRUE(isDecoded);
}

TEST_F(TestUdpMulticastMessage, willNotDecodeFromJsonIfNoSessionId)
{
  // Note the message does not include ip address (the sender doesn't know its external ip)
  const std::string jsonMessage =
      "{\"SeId\":\"7c9c0442-e1d6-4b16-81fe-f336cf967235\",\"TcpServerPort\":\"5775\"}";
  bool isDecoded;
  msg_.decodeMessage(jsonMessage, isDecoded);

  EXPECT_FALSE(isDecoded);
}

TEST_F(TestUdpMulticastMessage, willNotDecodeFromJsonIfNoServerPortNumber)
{
  bool isDecoded;
  // Note the message does not include ip address (the sender doesn't know its external ip)
  const std::string jsonMessage =
      "{\"SeId\":\"7c9c0442-e1d6-4b16-81fe-f336cf967235\",\"RdsServerPort\":\"5775\"}";
  msg_.decodeMessage(jsonMessage, isDecoded);

  EXPECT_FALSE(isDecoded);
}

TEST_F(TestUdpMulticastMessage, willNotDecodeFromJsonIfServerPortNumberIsNotInteger)
{
  // Note the message does not include ip address (the sender doesn't know its external ip)
  const std::string jsonMessage =
      "{\"SeId\":\"7c9c0442-e1d6-4b16-81fe-f336cf967235\",\"TcpServerPort\":\"beef\"}";
  bool isDecoded;
  msg_.decodeMessage(jsonMessage, isDecoded);

  EXPECT_FALSE(isDecoded);
}
