/*
 *  Communications Node
 *  @author Andrei Polzounov
 */
#include "TcpTimestampMessage.h"

namespace
{
  const uint64_t DEFAULT_TIMESTAMP = 0u;
}

TcpTimestampMessage::TcpTimestampMessage()
: timestamp_(DEFAULT_TIMESTAMP)
{
}

uint64_t TcpTimestampMessage::timestamp() const
{
  return timestamp_;
}

void TcpTimestampMessage::setTimestamp(uint64_t timestamp)
{
  timestamp_ = timestamp;
}

TcpTimestampMessage::ByteVector TcpTimestampMessage::encodeMessage() const
{
  // Fill out the ByteVector with 8 bytes in little endian format
  ByteVector data;
  data.resize(sizeof(timestamp_));
  int byteIndex = sizeof(timestamp_) - 1;
  uint64_t byteZeroMask = 0xffu;
  for(auto itr = data.begin(), end = data.end(); itr != end; ++itr, --byteIndex)
  {
    uint64_t currentByteBitMask = byteZeroMask << byteIndex*8;
    uint64_t currentByte = (timestamp_ & currentByteBitMask) >> byteIndex*8;
    *itr = static_cast<unsigned char>(currentByte);
  }
  return data;
}

void TcpTimestampMessage::decodeMessage(const ByteVector& data, bool& messageDecoded)
{
  if(data.size() == sizeof(timestamp_))
  {
    timestamp_ = 0u;
    int byteIndex = sizeof(timestamp_) - 1;
    for(auto itr = data.begin(), end = data.end(); itr != end; ++itr, --byteIndex)
    {
      uint64_t currentByte = static_cast<uint64_t>(*itr);
      uint64_t currentByteInPosition = (currentByte << byteIndex*8);
      timestamp_ = timestamp_ | currentByteInPosition;
    }
    messageDecoded = true;
  }
  else
  {
    messageDecoded = false;
  }
}
