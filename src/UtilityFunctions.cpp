/*
 *  Communications Node
 *  @author Andrei Polzounov
 */

// Workaround for a Windows warning with Boost UUID library
#ifdef _WIN32
#pragma GCC diagnostic ignored "-Wconversion-null"
#endif

#include <boost/date_time/gregorian/greg_calendar.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid.hpp>

#include "UtilityFunctions.h"

namespace UtilityFunctions
{
  std::string generateUuid()
  {
    auto generator = boost::uuids::random_generator();
    auto uuid = generator();
    return to_string(uuid);
  }

  uint64_t microsecondsSinceEpoch()
  {
    static const boost::posix_time::ptime epoch(boost::gregorian::date(1970, 1, 1));
    auto now = (boost::posix_time::microsec_clock::universal_time() - epoch).total_microseconds();
    return static_cast<uint64_t>(now);
  }
}
