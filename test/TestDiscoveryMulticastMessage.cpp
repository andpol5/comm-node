/*
 *  Communications Node Test
 *  @author Andrei Polzounov
 */
#include "gtest/gtest.h"

#include "DiscoveryMulticastMessage.h"

namespace
{
  // According to RFC 5737 this IP Address is reserved for documentation purposes
  const std::string DEFAULT_ADDRESS = "192.0.2.0";
  const std::string DEFAULT_SESSION_ID = "InvalidSessionId";
  const int DEFAULT_PORT = -1;

  class TestDiscoveryMulticastMessage : public ::testing::Test
  {
  protected:
    DiscoveryMulticastMessage msg_;
  };
}

TEST_F(TestDiscoveryMulticastMessage, valuesIdInvalidBeforeSet)
{
  EXPECT_EQ(msg_.isSessionIdValid(), false);
  EXPECT_EQ(msg_.isTcpServerAddressValid(), false);
  EXPECT_EQ(msg_.isTcpServerPortValid(), false);
}

TEST_F(TestDiscoveryMulticastMessage, defaultValues)
{
  EXPECT_EQ(msg_.sessionId(), DEFAULT_SESSION_ID);
  EXPECT_EQ(msg_.tcpServerAddress(), boost::asio::ip::address::from_string(DEFAULT_ADDRESS));
  EXPECT_EQ(msg_.tcpServerPort(), DEFAULT_PORT);
}

TEST_F(TestDiscoveryMulticastMessage, valuesValidatedAndGettableAfterSet)
{
  msg_.setSessionId("someId");
  msg_.setTcpServerAddress("192.167.4.55");
  msg_.setTcpServerPort(5864);

  EXPECT_EQ(msg_.isSessionIdValid(), true);
  EXPECT_EQ(msg_.isTcpServerAddressValid(), true);
  EXPECT_EQ(msg_.isTcpServerPortValid(), true);

  EXPECT_EQ(msg_.sessionId(), "someId");
  EXPECT_EQ(msg_.tcpServerAddress(), boost::asio::ip::address::from_string("192.167.4.55"));
  EXPECT_EQ(msg_.tcpServerPort(), 5864);
}

TEST_F(TestDiscoveryMulticastMessage, decodeFromJson)
{
  bool isDecoded;
  // Note the message does not include ip address (the sender doesn't know its external ip)
  const std::string jsonMessage =
      "{\"SessionId\":\"7c9c0442-e1d6-4b16-81fe-f336cf967235\",\"TcpServerPort\":\"5775\"}";
  msg_.decodeMessage(jsonMessage, isDecoded);

  EXPECT_EQ(msg_.isSessionIdValid(), true);
  EXPECT_EQ(msg_.isTcpServerAddressValid(), false);
  EXPECT_EQ(msg_.isTcpServerPortValid(), true);
  EXPECT_EQ(msg_.sessionId(), "7c9c0442-e1d6-4b16-81fe-f336cf967235");
  EXPECT_EQ(msg_.tcpServerPort(), 5775);
  EXPECT_TRUE(isDecoded);
}

TEST_F(TestDiscoveryMulticastMessage, encodeToJson)
{
  bool isEncoded;
  msg_.setSessionId("2c9c0570-1c01-1010-f336cf967235");
  msg_.setTcpServerPort(1337);

  const std::string expectedJsonMessage =
        "{\"SessionId\":\"2c9c0570-1c01-1010-f336cf967235\",\"TcpServerPort\":\"1337\"}";

  std::string data = msg_.encodeMessage(isEncoded);
  EXPECT_EQ(data, expectedJsonMessage);
  EXPECT_TRUE(isEncoded);
}
