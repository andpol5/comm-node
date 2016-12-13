/*
 *  Communications Node
 *  @author Andrei Polzounov
 */

// Workaround for a Windows warning with Boost UUID library
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
    auto generator = boost::uuids::random_generator();
    auto uuid = generator();
    return to_string(uuid);
  }
}
