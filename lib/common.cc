#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include <trafficgen/common.h>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace gr {
	namespace trafficgen {
		
		uint64_t calculate_timestamp(){

			boost::posix_time::ptime epoch(boost::gregorian::date(1970,1,1));
			boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
			boost::posix_time::time_duration diff = now - epoch;
			uint64_t timestamp = diff.total_milliseconds();

			return timestamp;
		}
		
	} /* namespace trafficgen */
} /* namespace gr */

