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

#include <cstdio>
#include <iostream>
#include <gnuradio/io_signature.h>
#include <string.h>
#include "time_trigger_impl.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>

namespace gr {

	namespace trafficgen {

		time_trigger::sptr
			time_trigger::make(bool initial_state, float active_time, float inactive_time){
	  	
	  	return gnuradio::get_initial_sptr 
	  		(new time_trigger_impl(initial_state, active_time, inactive_time));
	}

		/*
		 * The private constructor
		 */
		time_trigger_impl::time_trigger_impl(bool initial_state, float active_time, float inactive_time)
	  		: gr::block("time_trigger", gr::io_signature::make(0, 0, 0), gr::io_signature::make(0, 0, 0)),
	  		  d_finished(false),
	  		  d_state(initial_state),
	  		  d_active_time(active_time),
	  		  d_inactive_time(inactive_time){

  			message_port_register_out(pmt::mp("trigger"));
		}

		/*
		 * Our virtual destructor.
		 */
		time_trigger_impl::~time_trigger_impl(){}

		bool time_trigger_impl::start(){

			d_finished = false;
			d_thread = boost::shared_ptr<gr::thread::thread> 
			(new gr::thread::thread(boost::bind(&time_trigger_impl::run, this)));

			return block::start();
		}

		bool time_trigger_impl::stop(){

			d_finished = true;
			d_thread->interrupt();
			d_thread->join();

			return block::stop();
		}

		void time_trigger_impl::run(){

			boost::posix_time::ptime last_tick = boost::posix_time::microsec_clock::local_time();
			boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
			boost::posix_time::time_duration diff;
			d_msg = d_state ? pmt::PMT_T : pmt::PMT_F;
			float interval;

			while(!d_finished){

				// Start with the time we defined on each state
				if (d_state){

					interval = d_active_time;
					d_msg = pmt::PMT_T;

				} else {

					interval = d_inactive_time;
					d_msg = pmt::PMT_F;
				}

				d_state = !d_state;

				now = boost::posix_time::microsec_clock::local_time();
				diff = now - last_tick;
				last_tick = now;

				message_port_pub(pmt::mp("trigger"), d_msg);

				// std::cout << "Interval: " << diff.total_milliseconds() << std::endl;  TODO REMOVE DEBUG

				boost::this_thread::sleep(boost::posix_time::milliseconds(interval));

				if(d_finished){
					return;
				}
			}
		}
	} /* namespace trafficgen */
} /* namespace gr */

