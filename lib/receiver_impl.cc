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

#include <gnuradio/io_signature.h>
#include <trafficgen/common.h>
#include "receiver_impl.h"

namespace gr {
	namespace trafficgen {

		receiver::sptr
		receiver::make(const char *filename){

	  		return gnuradio::get_initial_sptr (new receiver_impl(filename));
		}

		receiver_impl::receiver_impl(const char *filename)
			: gr::block("receiver", gr::io_signature::make(0, 0, 0), gr::io_signature::make(0, 0, 0)),
			  stat_packets_total_received(0),
			  stat_packets_valid_received(0),
			  stat_packets_invalid_header(0),
			  stat_packets_invalid_crc(0),
			  stat_bytes_total_received(0),
			  stat_bytes_valid_received(0),
			  stat_throughput_average(0),
			  stat_jitter_average(0),
			  stat_latency_average(0),
			  stat_aggregate_latency(0),
			  stat_latency_samples(0),
			  stat_reception_started(boost::posix_time::not_a_date_time),
			  stat_reception_ended(boost::posix_time::not_a_date_time),
			  stat_last_packet_reception(boost::posix_time::not_a_date_time),
			  stat_last_packet_interval(boost::posix_time::not_a_date_time),
			  stat_aggregate_jitter(0),
			  stat_jitter_samples(0){

			open_logfile(filename);
			d_input_in_port = pmt::mp("input");
			d_ack_out_port = pmt::mp("ack");

			message_port_register_in(d_input_in_port);
			message_port_register_out(d_ack_out_port);

			set_msg_handler(d_input_in_port,
							boost::bind(&receiver_impl::handle_incoming_packet, this, _1));
		}

		receiver_impl::~receiver_impl(){}

		bool receiver_impl::start(){

			return block::start();
		}

		bool receiver_impl::stop(){

			stat_reception_ended = boost::posix_time::microsec_clock::local_time();
			boost::posix_time::time_duration time_diff = stat_reception_ended - stat_reception_started;

			stat_throughput_average = (stat_bytes_valid_received * 8.0) / ((double) time_diff.total_milliseconds()/1000.0);

			stat_jitter_average  = stat_aggregate_jitter / stat_jitter_samples;
			stat_latency_average =  stat_aggregate_latency / stat_latency_samples;

			d_logfile << "\n---- Finished ----\n" << std::flush;
			d_logfile << "packets_total_received;packets_valid_received;packets_invalid_header;packets_invalid_crc;bytes_total_received;bytes_valid_received;average_throughput_bps;average_latency_ms;average_jitter_ms;duration_ms\n" << std::flush;
			d_logfile << stat_packets_total_received << ";"
					  << stat_packets_valid_received << ";"
					  << stat_packets_invalid_header << ";"
					  << stat_packets_invalid_crc << ";"
					  << stat_bytes_total_received << ";"
					  << stat_bytes_valid_received << ";"
					  << stat_throughput_average << ";"
					  << stat_latency_average << ";"
					  << stat_jitter_average << ";"
					  << time_diff.total_milliseconds() << "\n" << std::flush;

			return block::stop();
		}

		void receiver_impl::open_logfile(const char *__filename){

			d_logfile.open(__filename, std::ios::out | std::ios::app);

			if (!d_logfile.is_open()){

				throw std::runtime_error("Unable to open file");
			}
		}

		void receiver_impl::handle_incoming_packet(pmt::pmt_t __packet){

			packet *pk = new packet(__packet);

			if (stat_reception_started.is_not_a_date_time()){

				stat_reception_started = boost::posix_time::microsec_clock::local_time();
			}

			boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();

			std::cout << "[RX][" << pk->get_id() 
						  << "][Size: " << pk->get_message_length() << " B]" << std::endl << std::flush;

			// pk->print();

			stat_packets_total_received++;
			stat_bytes_total_received += pk->get_message_length();

			bool check_result = pk->check();

			if (check_result == PK_VALID){

				uint64_t timestamp_now = calculate_timestamp();

				// Using abs() to avoid negative numbers in case of unsynchronized clocks
				stat_aggregate_latency += abs(timestamp_now - pk->get_timestamp());
				stat_latency_samples++;

				stat_packets_valid_received++;
				stat_bytes_valid_received += pk->get_message_length();

				if (!stat_last_packet_reception.is_not_a_date_time()){

					// Inteval between current and last packet
					boost::posix_time::time_duration diff = now - stat_last_packet_reception;
					double current = (double)diff.total_milliseconds();
					double last;

					if (!stat_last_packet_interval.is_not_a_date_time()){

						last = (double)stat_last_packet_interval.total_milliseconds();

						// Difference of intervals
						stat_aggregate_jitter += fabs(current - last);
						stat_jitter_samples++;
					}

					stat_last_packet_interval = diff;
				} 

				stat_last_packet_reception = now;

			} else if (check_result == PK_HEADER_ERR){

				stat_packets_invalid_header++;

			} else if (check_result == PK_CRC_ERR){

				stat_packets_invalid_crc++;
			}

			delete pk;
		}
	} /* namespace trafficgen */
} /* namespace gr */

