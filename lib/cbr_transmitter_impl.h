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

#ifndef INCLUDED_TRAFFICGEN_CBR_TRANSMITTER_IMPL_H
#define INCLUDED_TRAFFICGEN_CBR_TRANSMITTER_IMPL_H

#include <trafficgen/cbr_transmitter.h>
#include <gnuradio/thread/thread.h>
#include <trafficgen/common.h>
#include <boost/random/variate_generator.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/poisson_distribution.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/weibull_distribution.hpp>
#include <boost/random/uniform_int.hpp>
#include <iostream>
#include <fstream>

namespace gr {
  	namespace trafficgen {

		class cbr_transmitter_impl : public cbr_transmitter	{

			private:
				boost::shared_ptr<gr::thread::thread> d_thread;
				boost::condition_variable d_run_packet_creation;
				boost::mutex d_mutex_condition;
				bool d_create_packets;
				bool d_finished;
				bool d_trigger_start;
				bool d_trigger_stop;
	  			uint32_t d_packet_size;
	  			bool d_use_acks;
	  			float d_packet_interval;
	  			int d_constant_value;
	  			content_t d_content_type;
	  			distribution_t d_distribution_type;
	  			int d_dist_min;
	  			int d_dist_max;
	  			int d_dist_mean;
	  			float d_dist_std;
	  			float d_dist_shape;
	  			float d_dist_scale;
	  			std::ofstream d_logfile;

				pmt::pmt_t d_trigger_start_in_port;
				pmt::pmt_t d_trigger_stop_in_port;
				pmt::pmt_t d_pdu_out_port;

				boost::mt19937 d_rng;

				boost::shared_ptr<
					boost::variate_generator <
						boost::mt19937, boost::uniform_int<>
					>
				> d_variate_uniform;

				boost::shared_ptr<
					boost::variate_generator <
						boost::mt19937, boost::random::normal_distribution<>
					>
				> d_variate_normal;

				boost::shared_ptr<
					boost::variate_generator <
						boost::mt19937, boost::random::poisson_distribution<>
					>
				> d_variate_poisson;

				boost::shared_ptr<
					boost::variate_generator <
						boost::mt19937, boost::random::weibull_distribution<>
					>
				> d_variate_weibull;

	  			void run();

			public:
				cbr_transmitter_impl(uint32_t packet_size,
									 float packet_interval,
									 bool use_acks,
									 content_t content_type,
									 int constant_value,
									 distribution_t distribution_type,
									 int distribution_min,
									 int distribution_max,
									 int distribution_mean,
									 float distribution_std,
									 float distribution_shape,
									 float distribution_scale,
									 const char *filename);

				~cbr_transmitter_impl();

				void handle_trigger_start(pmt::pmt_t msg);

				void handle_trigger_stop(pmt::pmt_t msg);

				void set_trigger(bool *trigger, pmt::pmt_t msg);

				float get_random_value(distribution_t distribution);

				void setup_random_number_generators();

				void fill_payload(uint8_t *__payload, uint32_t __size);

				void open_logfile(const char *__filename);

				bool start();

				bool stop();
		};
	} // namespace trafficgen
} // namespace gr

#endif /* INCLUDED_TRAFFICGEN_CBR_TRANSMITTER_IMPL_H */

