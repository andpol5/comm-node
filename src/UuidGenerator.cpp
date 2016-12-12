/*
 *  Communications Node
 *  @author Andrei Polzounov
 */
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid.hpp>

#include "UuidGenerator.h"

namespace UuidGenerator
{
  std::string generate()
  {
    auto uuid = boost::uuids::random_generator()();
    return to_string(uuid);
  }
}
