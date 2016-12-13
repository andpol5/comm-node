/*
 *  Communications Node
 *  @author Andrei Polzounov
 */

// Workaround for Windows warning with Boost UUID library
#ifdef _WIN32
#pragma GCC diagnostic ignored "-Wconversion-null"
#endif

#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid.hpp>

#include "UuidGenerator.h"

namespace UuidGenerator
{
  std::string generate()
  {
	boost::mt19937 rand;
    boost::uuids::basic_random_generator<boost::mt19937> gen(&rand);
	boost::uuids::uuid u = gen();
    return to_string(u);
  }
}
