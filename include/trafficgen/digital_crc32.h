/* -*- c++ -*- */
/*
 * Copyright 2005,2011,2012 Free Software Foundation, Inc.
 * 
 * This file is part of GNU Radio
 * 
 * GNU Radio is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * GNU Radio is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with GNU Radio; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_TRAFFICGEN_DIGITAL_CRC32_H
#define INCLUDED_TRAFFICGEN_DIGITAL_CRC32_H

#include <trafficgen/api.h>

namespace gr {
	namespace trafficgen {

		// 	class TRAFFICGEN_API digital_crc32 {
		// public:
		//   digital_crc32();
		//   ~digital_crc32();
		// private:
		// };

	    TRAFFICGEN_API unsigned int 
    		update_crc32(unsigned int crc, const unsigned char *buf, size_t len);

    	TRAFFICGEN_API unsigned int 
    		update_crc32(unsigned int crc, const std::string buf);

    	TRAFFICGEN_API unsigned int 
    		crc32(const unsigned char *buf, size_t len);

    	TRAFFICGEN_API unsigned int 
    		crc32(const std::string buf);

	} // namespace trafficgen
} // namespace gr

#endif /* INCLUDED_TRAFFICGEN_DIGITAL_CRC32_H */

