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
#include "common/trafficgen.h"
#include "receiver_b_impl.h"
#include <cstdio>

namespace gr {

	namespace trafficgen {

		receiver_b::sptr
		receiver_b::make(bool use_acks){

	  		return gnuradio::get_initial_sptr(new receiver_b_impl(use_acks));
		}

		/*
		 * The private constructor
		 */
		receiver_b_impl::receiver_b_impl(bool use_acks)
	  		: gr::sync_block("receiver_b",
							 gr::io_signature::make(1, 1, sizeof(uint8_t)),
							 gr::io_signature::make(0, 0, 0)){

	  			message_port_register_in(MSG_PORT_ID);
	  			set_msg_handler(MSG_PORT_ID, boost::bind(&receiver_b_impl::message_handler, this, _1));

	  		}

		/*
		 * Our virtual destructor.
		 */
		receiver_b_impl::~receiver_b_impl(){}

		int	receiver_b_impl::work(int noutput_items,
								  gr_vector_const_void_star &input_items,
								  gr_vector_void_star &output_items){

	  // 		const uint8_t *in = (const uint8_t *) input_items[0];

			// // std::cout << "Received: " << (char)in[0] << std::endl;
			// printf("Received %d\n", in[0]);

			noutput_items = 1;
			consume_each(noutput_items);

	  // 		// Tell runtime system how many output items we produced.
	  		return noutput_items;
		}

		void receiver_b_impl::message_handler(pmt::pmt_t msg){

			std::cout << "receiver_b_impl::message_handler" << std::endl;

			printf("msg_handler: %lu", pmt::to_long(msg));

		}

	} /* namespace trafficgen */
} /* namespace gr */

