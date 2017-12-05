#ifndef INCLUDED_TRAFFICGEN_RECEIVER_IMPL_H
#define INCLUDED_TRAFFICGEN_RECEIVER_IMPL_H

#include <trafficgen/receiver.h>
#include <trafficgen/packet.h>
#include <iostream>
#include <fstream>

namespace gr {
	namespace trafficgen {

		class receiver_impl : public receiver {

		private:
			std::ofstream d_logfile;
			uint64_t stat_packets_total_received;
			uint64_t stat_packets_valid_received;
			uint64_t stat_packets_invalid_header;
			uint64_t stat_packets_invalid_crc;
			uint64_t stat_bytes_total_received;
			uint64_t stat_bytes_valid_received;
			double   stat_throughput_average;
			double   stat_jitter_average;
			double   stat_latency_average;

			boost::posix_time::ptime reception_started;
			boost::posix_time::ptime reception_ended;
			boost::posix_time::ptime last_packet_reception;
			boost::posix_time::time_duration last_packet_interval;
			double   aggregate_jitter;
			uint64_t jitter_samples;

			pmt::pmt_t d_input_in_port;
			pmt::pmt_t d_ack_out_port;

		public:
			receiver_impl(const char *filename);
			~receiver_impl();

			bool start();

			bool stop();

			void open_logfile(const char *__filename);

			void handle_incoming_packet(pmt::pmt_t __packet);
		};
	} // namespace trafficgen
} // namespace gr

#endif /* INCLUDED_TRAFFICGEN_RECEIVER_IMPL_H */

