/* -*- c++ -*- */
/* 
 * Copyright 2017 Gilson Miranda Jr <junior.kdm@gmail.com>
 * Copyright 2017 Matheus H. N. Nunes <mhnnunes@dcc.ufmg.br>
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

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

