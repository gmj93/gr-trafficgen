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

#include <cstdio>
#include <gnuradio/io_signature.h>
#include "cbr_transmitter_impl.h"
#include <trafficgen/trafficgen_common.h>

namespace gr {
	namespace trafficgen {

		cbr_transmitter::sptr
		cbr_transmitter::make (uint32_t packet_size,
							   float packet_interval,
							   trafficgen_content_t content_type,
							   int constant_value,
							   trafficgen_random_distribution_t distribution_type,
							   int distribution_min,
							   int distribution_max,
							   int distribution_mean,
							   float distribution_std){

			return gnuradio::get_initial_sptr (
				new cbr_transmitter_impl(
					packet_size,
					packet_interval,
					content_type,
					constant_value,
					distribution_type,
					distribution_min,
					distribution_max,
					distribution_mean,
					distribution_std
				)
			);
		}

		/* Constructor */
		cbr_transmitter_impl::cbr_transmitter_impl(uint32_t packet_size,
												   float packet_interval,
												   trafficgen_content_t content_type,
												   int constant_value,
												   trafficgen_random_distribution_t distribution_type,
												   int distribution_min,
												   int distribution_max,
												   int distribution_mean,
												   float distribution_std)
			: gr::block("cbr_transmitter", gr::io_signature::make(0, 0, 0), gr::io_signature::make(0, 0, 0)),
			  d_finished(false),
			  d_trigger_start(true),
			  d_trigger_stop(false),
			  d_packet_size(packet_size),
			  d_packet_interval(packet_interval),
			  d_content_type(content_type),
			  d_constant_value(constant_value),
			  d_distribution_type(distribution_type),
			  d_dist_min(distribution_min),
			  d_dist_max(distribution_max),
			  d_dist_mean(distribution_mean),
			  d_dist_std(distribution_std),
			  d_rng(){

			d_trigger_start_in_port = pmt::mp("Trigger Start");
			d_trigger_stop_in_port = pmt::mp("Trigger Stop");
			d_pdu_out_port = pmt::mp("pdu");

			message_port_register_in(d_trigger_start_in_port);
			message_port_register_in(d_trigger_stop_in_port);
			message_port_register_out(d_pdu_out_port);

			set_msg_handler(d_trigger_start_in_port,
							boost::bind(&cbr_transmitter_impl::handle_trigger_start, this, _1));

			set_msg_handler(d_trigger_stop_in_port,
							boost::bind(&cbr_transmitter_impl::handle_trigger_stop, this, _1));

			setup_random_number_generators();
		}

		/* Destructor. */
		cbr_transmitter_impl::~cbr_transmitter_impl(){}

		void cbr_transmitter_impl::handle_trigger_start(pmt::pmt_t msg){

			set_trigger(&d_trigger_start, msg);
		}

		void cbr_transmitter_impl::handle_trigger_stop(pmt::pmt_t msg){

			set_trigger(&d_trigger_stop, msg);
		}

		void cbr_transmitter_impl::set_trigger(bool *trigger, pmt::pmt_t msg){

			*trigger = (msg == pmt::PMT_T ? true : false);

			if (d_trigger_start && !d_trigger_stop){

				d_run_packet_creation.notify_one();
				d_create_packets = true;

			} else {

				d_create_packets = false;
			}
		}

		float cbr_transmitter_impl::get_random_value(trafficgen_random_distribution_t distribution){

			switch(distribution){
				case DIST_UNIFORM:
					return d_variate_uniform->operator()();
				case DIST_GAUSSIAN:
					return d_variate_normal->operator()();
				case DIST_POISSON:
					return d_variate_poisson->operator()();
				default:
					throw std::runtime_error("Unknown ditribution defined");
			}
		}

		void cbr_transmitter_impl::setup_random_number_generators(){

			boost::uniform_int<> ud(d_dist_min, d_dist_max);
			d_variate_uniform = boost::shared_ptr<boost::variate_generator<boost::mt19937, boost::uniform_int<>>>(
				new boost::variate_generator <boost::mt19937, boost::uniform_int<>>(d_rng, ud));

			boost::normal_distribution<> nd(d_dist_mean, d_dist_std);
			d_variate_normal = boost::shared_ptr<boost::variate_generator<boost::mt19937, boost::normal_distribution<>>>(
				new boost::variate_generator <boost::mt19937, boost::normal_distribution<>>(d_rng, nd));

			boost::poisson_distribution<> pd(d_dist_mean);
			d_variate_poisson = boost::shared_ptr< boost::variate_generator<boost::mt19937, boost::poisson_distribution<>>>(
				new boost::variate_generator <boost::mt19937, boost::poisson_distribution<>>(d_rng, pd));
		}

		bool cbr_transmitter_impl::start(){

			d_finished = false;
			d_thread = boost::shared_ptr<gr::thread::thread> 
			(new gr::thread::thread(boost::bind(&cbr_transmitter_impl::run, this)));

			return block::start();
		}

		bool cbr_transmitter_impl::stop(){

			d_finished = true;
			d_run_packet_creation.notify_one();
			d_thread->interrupt();
			d_thread->join();

			return block::stop();
		}

		void cbr_transmitter_impl::run(){

			while(!d_finished){

				std::cout << "------ CBR DEBUG ----------" << std::endl;
				std::cout << "packet_size: " << d_packet_size << std::endl;
				std::cout << "packet_interval: " << d_packet_interval << std::endl;
				std::cout << "content_type: " << d_content_type << std::endl;
				std::cout << "constant_value: " << d_constant_value << std::endl;
				std::cout << "distribution_type: " << d_distribution_type << std::endl;
				std::cout << "trigger_start: " << d_trigger_start << std::endl;
				std::cout << "trigger_stop: " << d_trigger_stop << std::endl;
				std::cout << "get_random_value (uniform): " << get_random_value(DIST_UNIFORM) << std::endl;
				std::cout << "get_random_value (gaussian): " << get_random_value(DIST_GAUSSIAN) << std::endl;
				std::cout << "get_random_value (poisson): " << get_random_value(DIST_POISSON) << std::endl;
				std::cout << "------ CBR DEBUG ----------" << std::endl;

				boost::this_thread::sleep(boost::posix_time::milliseconds(d_packet_interval));

				if (!d_create_packets) {

					boost::mutex::scoped_lock lock(d_mutex_condition);
					d_run_packet_creation.wait(lock);	
				}

				if (d_finished){
					return;
				}
			}
		}
	} /* namespace trafficgen */
} /* namespace gr */

