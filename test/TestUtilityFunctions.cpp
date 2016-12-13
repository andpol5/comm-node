/*
 *  Communications Node
 *  @author Andrei Polzounov
 */
#include <set>
#include <vector>

#include "gtest/gtest.h"

#include "UtilityFunctions.h"

using namespace std;

TEST(TestUtilityFunctions, allUuidsAreUnique)
{
  // Quantitative test to see if N ids will all be different from each other
  const unsigned int numberOfIdsToGenerate = 500;
  vector<string> ids;
  ids.resize(numberOfIdsToGenerate);
  for(auto itr = ids.begin(), end = ids.end(); itr != end; ++itr)
  {
    *itr = UtilityFunctions::generateUuid();
  }

  // Use std::set to count the distinct values
  std::set<std::string> distinctContainer;
  for(auto itr = ids.begin(), end = ids.end(); itr != end; ++itr)
  {
    distinctContainer.insert(*itr);
  }

  EXPECT_EQ(distinctContainer.size(), numberOfIdsToGenerate);
}

TEST(TestUtilityFunctions, microsecondClocksAreIncreasing)
{
  // Quantitative test to see if N timestamps will increase
  const int numberOfTests = 500;

  uint64_t previousTime = UtilityFunctions::microsecondsSinceEpoch();
  for(int i = 0; i < numberOfTests; ++i)
  {
    uint64_t currentTime = UtilityFunctions::microsecondsSinceEpoch();
    EXPECT_LT(previousTime, currentTime);
  }
}
