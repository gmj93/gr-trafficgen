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
#include <trafficgen/common.h>
#include <trafficgen/packet.h>
#include <cstring>
#include <cstdio>
#include "cbr_transmitter_impl.h"

namespace gr {
	namespace trafficgen {

		cbr_transmitter::sptr
		cbr_transmitter::make (uint32_t packet_size,
							   float packet_interval,
							   bool use_acks,
							   trafficgen_content_t content_type,
							   int constant_value,
							   trafficgen_random_distribution_t distribution_type,
							   int distribution_min,
							   int distribution_max,
							   int distribution_mean,
							   float distribution_std,
							   float distribution_shape,
							   float distribution_scale,
							   const char *filename){

			return gnuradio::get_initial_sptr (
				new cbr_transmitter_impl(
					packet_size,
					packet_interval,
					use_acks,
					content_type,
					constant_value,
					distribution_type,
					distribution_min,
					distribution_max,
					distribution_mean,
					distribution_std,
					distribution_shape,
					distribution_scale,
					filename
				)
			);
		}

		/* Constructor */
		cbr_transmitter_impl::cbr_transmitter_impl(uint32_t packet_size,
												   float packet_interval,
												   bool use_acks,
												   trafficgen_content_t content_type,
												   int constant_value,
												   trafficgen_random_distribution_t distribution_type,
												   int distribution_min,
												   int distribution_max,
												   int distribution_mean,
												   float distribution_std,
												   float distribution_shape,
												   float distribution_scale,
												   const char *filename)
			: gr::block("cbr_transmitter", gr::io_signature::make(0, 0, 0), gr::io_signature::make(0, 0, 0)),
			  d_finished(false),
			  d_trigger_start(true),
			  d_trigger_stop(false),
			  d_packet_size(packet_size),
			  d_packet_interval(packet_interval),
			  d_use_acks(use_acks),
			  d_content_type(content_type),
			  d_constant_value(constant_value),
			  d_distribution_type(distribution_type),
			  d_dist_min(distribution_min),
			  d_dist_max(distribution_max),
			  d_dist_mean(distribution_mean),
			  d_dist_std(distribution_std),
			  d_dist_shape(distribution_shape),
			  d_dist_scale(distribution_scale),
			  d_rng(){

			open_logfile(filename);

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
				case DIST_WEIBULL:
					return d_variate_weibull->operator()();
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

			boost::random::weibull_distribution<> wd(d_dist_shape, d_dist_scale);
			d_variate_weibull = boost::shared_ptr< boost::variate_generator<boost::mt19937, boost::random::weibull_distribution<>>>(
				new boost::variate_generator <boost::mt19937, boost::random::weibull_distribution<>>(d_rng, wd));
		}

		void cbr_transmitter_impl::fill_payload(uint8_t *__payload, uint32_t __size){
			
			switch(d_content_type){

				case CONTENT_CONSTANT: {

					std::memset(__payload, d_constant_value, (__size * sizeof(uint8_t)));
				}
				break;

				case CONTENT_RANDOM: {

					for (uint32_t i = 0; i < __size; i++){

						__payload[i] = get_random_value(d_distribution_type);
					}
				}
				break;

				case CONTENT_SEQUENTIAL: {

					uint32_t copied = 0;
					uint32_t amount_to_copy = 0;
					uint32_t element_count = (d_dist_max - d_dist_min + 1);
					uint8_t elements[element_count];

					std::iota(elements, elements + element_count, d_dist_min);

					for (uint32_t i = 0; i < __size; i++){

						amount_to_copy = (__size - copied);
						amount_to_copy = amount_to_copy < element_count ? amount_to_copy : element_count;

						std::memcpy(__payload + copied, &elements, amount_to_copy);

						copied += amount_to_copy;
					}
				}
				break;

				default:
				throw std::runtime_error("Undefined content type");
			}
		}

		void cbr_transmitter_impl::open_logfile(const char *__filename){

			d_logfile.open(__filename, std::ios::out | std::ios::app);

			if (!d_logfile.is_open()){

				throw std::runtime_error("Unable to open file");
			}
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

			boost::this_thread::disable_interruption di;

			uint32_t id = 0;
			packet *pk = new packet(PACKET_HEADER, d_use_acks, id, d_packet_size);	// Setup new packet
			uint32_t payload_length = pk->get_payload_length();						// Get resulting payload space in it
			uint8_t *payload = new uint8_t[payload_length];							// Setup a new payload

			fill_payload(payload, payload_length);							// Fill payload with selected content

			pk->set_payload(payload, pk->get_payload_length());				// Set our payload into packet

			pmt::pmt_t blob_packet;
			uint32_t stat_sent_packets = 0;
  			uint64_t stat_sent_bytes = 0;
  			double stat_average_throughput = 0;
  			boost::posix_time::time_duration time_diff;
  			boost::posix_time::ptime start;
  			boost::posix_time::ptime stop;

  			start = boost::posix_time::microsec_clock::local_time();

			while(!d_finished){

				boost::this_thread::sleep(boost::posix_time::milliseconds(d_packet_interval));

				blob_packet = pk->get_blob();
				message_port_pub(d_pdu_out_port, blob_packet);

				stat_sent_packets++;
				stat_sent_bytes += d_packet_size;

				if (d_content_type == CONTENT_RANDOM){

					fill_payload(payload, payload_length);
					pk->generate_next(payload);

				} else {

					pk->generate_next();
				}

				if (!d_create_packets) {

					stop = boost::posix_time::microsec_clock::local_time();

					time_diff = stop - start;
					stat_average_throughput = (double)stat_sent_bytes * 8.0 / (double)time_diff.total_milliseconds();

					d_logfile << "\n---- Trigger interruption ----\n" << std::flush;
					d_logfile << "packets_sent;packets_received;tx_duration_ms;throughput\n" << std::flush;
					d_logfile << stat_sent_packets << ";" 
							  << stat_sent_bytes << ";" 
							  << time_diff.total_milliseconds() << ";"
							  << stat_average_throughput << "\n" 
							  << std::flush;

					boost::mutex::scoped_lock lock(d_mutex_condition);
					d_run_packet_creation.wait(lock);
					start = boost::posix_time::microsec_clock::local_time();
					stat_sent_packets = 0;
					stat_sent_bytes = 0;
					stat_average_throughput = 0;
				}

				if (d_finished){

					stop = boost::posix_time::microsec_clock::local_time();

					time_diff = stop - start;
					stat_average_throughput = (double)stat_sent_bytes * 8.0 / (double)time_diff.total_milliseconds();

					d_logfile << "\n---- Finished ----\n" << std::flush;
					d_logfile << "packets_sent;packets_received;tx_duration_ms;throughput\n" << std::flush;
					d_logfile << stat_sent_packets << ";" 
							  << stat_sent_bytes << ";" 
							  << time_diff.total_milliseconds() << ";"
							  << stat_average_throughput << "\n" 
							  << std::flush;

					d_logfile.close();

					delete pk;
					delete payload;
					return;
				}
			}
		}
	} /* namespace trafficgen */
} /* namespace gr */

