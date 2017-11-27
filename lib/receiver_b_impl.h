/* -*- c++ -*- */
/* 
 * Copyright 2017 <+YOU OR YOUR COMPANY+>.
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

#ifndef INCLUDED_TRAFFICGEN_RECEIVER_B_IMPL_H
#define INCLUDED_TRAFFICGEN_RECEIVER_B_IMPL_H

#include <trafficgen/receiver_b.h>

namespace gr {

	namespace trafficgen {

		class receiver_b_impl : public receiver_b {

			private:

			public:
				receiver_b_impl(bool use_acks);
				~receiver_b_impl();

			// Where all the action really happens
				int work(int noutput_items,
						 gr_vector_const_void_star &input_items,
						 gr_vector_void_star &output_items);

				void message_handler(pmt::pmt_t msg);
		};
	} // namespace trafficgen
} // namespace gr

#endif /* INCLUDED_TRAFFICGEN_RECEIVER_B_IMPL_H */
