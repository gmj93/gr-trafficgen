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

#ifndef INCLUDED_TRAFFICGEN_VBR_TRANSMITTER_H
#define INCLUDED_TRAFFICGEN_VBR_TRANSMITTER_H

#include <trafficgen/api.h>
#include <gnuradio/block.h>
#include <trafficgen/common.h>

namespace gr {
	namespace trafficgen {

		class TRAFFICGEN_API vbr_transmitter : virtual public gr::block	{
			public:
				typedef boost::shared_ptr<vbr_transmitter> sptr;

				virtual void open_logfile(const char *__filename) = 0;

				static sptr make(bool use_acks,
								 content_t content_type,
								 int constant_value,
								 int sequential_minimum,
								 int sequential_maximum,
								 const char *logfile);
		};
	} // namespace trafficgen
} // namespace gr

#endif /* INCLUDED_TRAFFICGEN_VBR_TRANSMITTER_H */

