#pragma once
/*
 *  Communications Node
 *  @author Andrei Polzounov
 */
#include <vector>

class CommNode;

class CommNodeUi
{
public:
  CommNodeUi();
  virtual ~CommNodeUi();

  void updateScreen(const std::vector<CommNode>& nodes) const;
};
