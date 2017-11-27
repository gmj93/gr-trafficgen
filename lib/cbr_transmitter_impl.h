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

#ifndef INCLUDED_TRAFFICGEN_CBR_TRANSMITTER_IMPL_H
#define INCLUDED_TRAFFICGEN_CBR_TRANSMITTER_IMPL_H

#include <trafficgen/cbr_transmitter.h>
#include <gnuradio/thread/thread.h>

namespace gr {
  	namespace trafficgen {

		class cbr_transmitter_impl : public cbr_transmitter	{

			private:

	  			bool d_trigger_start;
	  			bool d_trigger_stop;
	  			uint8_t d_content;

	  			pmt::pmt_t d_trigger_start_in_port;
	  			pmt::pmt_t d_trigger_stop_in_port;
	  			pmt::pmt_t d_content_in_port;

			public:
				cbr_transmitter_impl(uint32_t packet_size,
									 double packet_interval,
									 double start_time,
									 double stop_time);

				~cbr_transmitter_impl();

				// Where all the action really happens
				void handle_trigger_start(pmt::pmt_t msg);

				void handle_trigger_stop(pmt::pmt_t msg);

				void forecast (int noutput_items, gr_vector_int &ninput_items_required);

				int general_work(int noutput_items,
								 gr_vector_int &ninput_items,
								 gr_vector_const_void_star &input_items,
								 gr_vector_void_star &output_items);
		};
	} // namespace trafficgen
} // namespace gr

#endif /* INCLUDED_TRAFFICGEN_CBR_TRANSMITTER_IMPL_H */

