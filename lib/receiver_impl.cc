#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
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
			  reception_started(boost::posix_time::not_a_date_time),
			  reception_ended(boost::posix_time::not_a_date_time),
			  last_packet_reception(boost::posix_time::not_a_date_time),
			  last_packet_interval(boost::posix_time::not_a_date_time),
			  aggregate_jitter(0),
			  jitter_samples(0){

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

			reception_ended = boost::posix_time::microsec_clock::local_time();
			boost::posix_time::time_duration time_diff = reception_ended - reception_started;

			stat_throughput_average = (stat_bytes_valid_received * 8.0) / ((double) time_diff.total_milliseconds()/1000.0);

			stat_jitter_average = aggregate_jitter / jitter_samples;

			d_logfile << "\n---- Finished ----\n" << std::flush;
			d_logfile << "packets_total_received;packets_valid_received;packets_invalid_header;packets_invalid_crc;bytes_total_received;bytes_valid_received;average_throughput_bps;average_jitter_ms;duration_ms\n" << std::flush;
			d_logfile << stat_packets_total_received << ";"
					  << stat_packets_valid_received << ";"
					  << stat_packets_invalid_header << ";"
					  << stat_packets_invalid_crc << ";"
					  << stat_bytes_total_received << ";"
					  << stat_bytes_valid_received << ";"
					  << stat_throughput_average << ";"
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

			if (reception_started.is_not_a_date_time()){

				reception_started = boost::posix_time::microsec_clock::local_time();
			}

			boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();

			stat_packets_total_received++;
			stat_bytes_total_received += pk->get_message_length();

			bool check_result = pk->check();

			if (check_result == PK_VALID){

				stat_packets_valid_received++;
				stat_bytes_valid_received += pk->get_message_length();

				if (!last_packet_reception.is_not_a_date_time()){

					// Inteval between current and last packet
					boost::posix_time::time_duration diff = now - last_packet_reception;
					double current = (double)diff.total_milliseconds();
					double last;

					if (!last_packet_interval.is_not_a_date_time()){

						last = (double)last_packet_interval.total_milliseconds();

						// Difference of intervals
						aggregate_jitter += fabs(current - last);
						jitter_samples++;
					}

					last_packet_interval = diff;
				} 

				last_packet_reception = now;

			} else if (check_result == PK_HEADER_ERR){

				stat_packets_invalid_header++;

			} else if (check_result == PK_CRC_ERR){

				stat_packets_invalid_crc++;
			}

			delete pk;
		}
	} /* namespace trafficgen */
} /* namespace gr */

