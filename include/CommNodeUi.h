#pragma once
/*
 *  Communications Node
 *  @author Andrei Polzounov
 */
#include <vector>

#include <boost/asio.hpp>

class CommNode;

class CommNodeUi
{
public:
  CommNodeUi(const std::string sessionId,
      const boost::asio::ip::address& multicastAddress, int multicastPort);
  virtual ~CommNodeUi();

  void updateScreen(const std::vector<CommNode>& nodes) const;

private:
  std::string titleLine_;
};
