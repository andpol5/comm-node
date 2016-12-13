/*
 *  Communications Node Test
 *  @author Andrei Polzounov
 */
#include "gtest/gtest.h"

#include "TcpTimestampMessage.h"
#include "UtilityFunctions.h"

namespace
{
  const uint64_t DEFAULT_TIMESTAMP = 0u;

  class TestTcpTimestampMessage : public ::testing::Test
  {
  protected:
    typedef std::vector<unsigned char> ByteVector;
    TcpTimestampMessage msg_;
  };
}

TEST_F(TestTcpTimestampMessage, willReturnDefaultValues)
{
  EXPECT_EQ(msg_.timestamp(), DEFAULT_TIMESTAMP);
}

TEST_F(TestTcpTimestampMessage, willGetNewValuesAfterSetting)
{
  msg_.setTimestamp(42424242u);

  EXPECT_EQ(msg_.timestamp(), 42424242u);
}

TEST_F(TestTcpTimestampMessage, willEncodeToByteVector)
{
  // ab 4e 5a 68 88 33 ce 75
  unsigned char buff[] = {0xabu, 0x4eu, 0x5au, 0x68u, 0x88u, 0x33u, 0xceu, 0x75u};
  unsigned char* buffPtr = &buff[0];
  ByteVector expectedData;
  expectedData.resize(8);
  expectedData.assign(buffPtr, buffPtr + 8);

  msg_.setTimestamp(0xab4e5a688833ce75u);
  ByteVector data = msg_.encodeMessage();
  EXPECT_EQ(data, expectedData);
}

TEST_F(TestTcpTimestampMessage, willDecodeFromByteVector)
{
  // ff4f2bde0ab8c7b7
  unsigned char buff[] = {0xffu, 0x4fu, 0x2bu, 0xdeu, 0x0au, 0xb8u, 0xc7u, 0xb7u};
  unsigned char* buffPtr = &buff[0];
  ByteVector data;
  data.resize(8);
  data.assign(buffPtr, buffPtr + 8);

  bool ok;
  msg_.decodeMessage(data, ok);
  EXPECT_TRUE(ok);
  EXPECT_EQ(msg_.timestamp(), 0xff4f2bde0ab8c7b7u);
}

TEST_F(TestTcpTimestampMessage, willEncodeAndDecodeRealTimestamps)
{
  uint64_t now = UtilityFunctions::microsecondsSinceEpoch();

  msg_.setTimestamp(now);
  ByteVector data = msg_.encodeMessage();
  TcpTimestampMessage otherMsg;
  bool ok;
  otherMsg.decodeMessage(data, ok);

  EXPECT_TRUE(ok);
  EXPECT_EQ(now, msg_.timestamp());
  EXPECT_EQ(now, otherMsg.timestamp());
  EXPECT_EQ(data, otherMsg.encodeMessage());
}
