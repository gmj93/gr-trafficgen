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
#include "user_trigger_impl.h"

namespace gr {

	namespace trafficgen {

		user_trigger::sptr
		user_trigger::make(bool state){
	  		
	  		return gnuradio::get_initial_sptr (new user_trigger_impl(state));
		}

		/*
		 * The private constructor
		 */
		user_trigger_impl::user_trigger_impl(bool state)
	  		: gr::block("user_trigger", gr::io_signature::make(0, 0, 0), gr::io_signature::make(0,0,0)),
			  d_finished(false),
			  d_state(state){

			  	message_port_register_out(pmt::mp("trigger"));
		}

		/*
		 * Our virtual destructor.
		 */
		user_trigger_impl::~user_trigger_impl(){}

		bool user_trigger_impl::start(){

			message_port_pub(pmt::mp("trigger"), (d_state ? pmt::PMT_T : pmt::PMT_F));

			return block::start();
		}

		bool user_trigger_impl::stop(){

			return block::stop();
		}

		void user_trigger_impl::set_state(bool state){

			message_port_pub(pmt::mp("trigger"), (state ? pmt::PMT_T : pmt::PMT_F));
		}
	} /* namespace trafficgen */
} /* namespace gr */

