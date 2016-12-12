/*
 *  Communications Node
 *  @author Andrei Polzounov
 */
#include <set>
#include <vector>

#include "gtest/gtest.h"

#include "UuidGenerator.h"

using namespace std;

TEST(TestUuidGenerator, allUuidsAreUnique)
{
  // Quantitative test to see if N ids will all be different from each other
  const unsigned int numberOfIdsToGenerate = 500;
  vector<string> ids;
  ids.resize(numberOfIdsToGenerate);
  for(auto itr = ids.begin(), end = ids.end(); itr != end; ++itr)
  {
    *itr = UuidGenerator::generate();
  }

  // Use std::set to count the distinct values
  std::set<std::string> distinctContainer;
  for(auto itr = ids.begin(), end = ids.end(); itr != end; ++itr)
  {
    distinctContainer.insert(*itr);
  }

  EXPECT_EQ(distinctContainer.size(), numberOfIdsToGenerate);
}
