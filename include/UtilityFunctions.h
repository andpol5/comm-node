#pragma once
/*
 *  Communications Node
 *  @author Andrei Polzounov
 */
#include <cstdint>
#include <string>

namespace UtilityFunctions
{
  std::string generateUuid();
  uint64_t microsecondsSinceEpoch();
};
