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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "cbr_transmitter_impl.h"
#include "common/packet_format.h"
#include "common/trafficgen.h"
#include <cstdio>

namespace gr {
	namespace trafficgen {

		cbr_transmitter::sptr
		cbr_transmitter::make (
			uint32_t packet_size,
			double packet_interval,
			double start_time,
			double stop_time
		){

  			return gnuradio::get_initial_sptr (
  				new cbr_transmitter_impl(
  					packet_size,
  					packet_interval,
  					start_time,
  					stop_time
				)
			);
		}

		/*
		 * The private constructor
		 */
		cbr_transmitter_impl::cbr_transmitter_impl(uint32_t packet_size,
												   double packet_interval,
												   double start_time,
												   double stop_time)
	  		: gr::block("cbr_transmitter",
			  gr::io_signature::make3(0, 3, sizeof(int), sizeof(int), sizeof(uint8_t)),
			  gr::io_signature::make(0, 1, sizeof(uint8_t))){	// TODO Change to 1, 1 later...

			d_trigger_start_in_port = pmt::mp("Trigger Start");
			d_trigger_stop_in_port = pmt::mp("Trigger Stop");
			d_content_in_port = pmt::mp("Content");

			message_port_register_in(d_trigger_start_in_port);
			message_port_register_in(d_trigger_stop_in_port);
			// message_port_register_in(d_content_in_port);
			message_port_register_out(MSG_PORT_ID);

			set_msg_handler(d_trigger_start_in_port,
							boost::bind(&cbr_transmitter_impl::handle_trigger_start, this, _1));

			set_msg_handler(d_trigger_stop_in_port,
							boost::bind(&cbr_transmitter_impl::handle_trigger_stop, this, _1));

			std::cout << gr::block::message_ports_in() << std::endl;
			std::cout << gr::block::message_ports_out() << std::endl;
  		}

		/*
		 * Our virtual destructor.
		 */
		cbr_transmitter_impl::~cbr_transmitter_impl(){}

		void cbr_transmitter_impl::handle_trigger_start(pmt::pmt_t msg){

			std::cout << "trigger_start: " << std::endl;
			pmt::print(msg);
		}

		void cbr_transmitter_impl::handle_trigger_stop(pmt::pmt_t msg){

			std::cout << "trigger_stop: " << std::endl;
			pmt::print(msg);
		}

		void cbr_transmitter_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required) {
			/* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
		}

		int	cbr_transmitter_impl::general_work (int noutput_items,
						   						gr_vector_int &ninput_items,
												gr_vector_const_void_star &input_items,
												gr_vector_void_star &output_items){

			const int *triggerStart = (const int *) input_items[0];
			const int *triggerStop = (const int *) input_items[1];
			const uint8_t *content = (const uint8_t *) input_items[2];

			printf("noutput_items: %d\n", noutput_items);
			printf("ninput_items: %d\n", ninput_items);
			printf("triggerStart: %d\n", *triggerStart);
			printf("triggerStop: %d\n", *triggerStop);
			printf("content: %d\n", *content);

			uint8_t *out = (uint8_t *) output_items[0];

			*out = 5;

			message_port_pub(MSG_PORT_ID, pmt::from_long(*out));

			noutput_items = 1;

			usleep(10000);

			// // Do <+signal processing+>
			// // Tell runtime system how many input items we consumed on
			// // each input stream.
			// consume_each (noutput_items);

			// Tell runtime system how many output items we produced.
			return noutput_items;
		}

  	} /* namespace trafficgen */
} /* namespace gr */

