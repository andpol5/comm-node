#pragma once
/*
 *  Communications Node
 *  @author Andrei Polzounov
 */
#include <cstdint>
#include <vector>

class TcpTimestampMessage
{
public:
  TcpTimestampMessage();

  // Timestamp in microseconds since epoch
  uint64_t timestamp() const;
  void setTimestamp(uint64_t sessionId);

  // Encode/Decode to/from raw 64 bytes (little endian format)
  typedef std::vector<unsigned char> ByteVector;
  ByteVector encodeMessage() const;
  void decodeMessage(const ByteVector& data, bool& messageDecoded);

private:
  uint64_t timestamp_;
};
