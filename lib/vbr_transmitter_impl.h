#ifndef INCLUDED_TRAFFICGEN_VBR_TRANSMITTER_IMPL_H
#define INCLUDED_TRAFFICGEN_VBR_TRANSMITTER_IMPL_H

#include <trafficgen/vbr_transmitter.h>
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

		class vbr_transmitter_impl : public vbr_transmitter {

			private:
				boost::shared_ptr<gr::thread::thread> d_thread;
				boost::condition_variable d_run_packet_creation;
				boost::mutex d_mutex_condition;
				bool d_create_packets;

				bool d_finished;
				bool d_trigger_start;
				bool d_trigger_stop;
				bool d_use_acks;
				content_t d_content_type;
				int d_constant_value;
				int d_sequential_minimum;
				int d_sequential_maximum;
				std::ofstream d_logfile;

				uint64_t d_packet_size;
				uint64_t d_burst_interval;
				uint64_t d_burst_duration;
				uint64_t d_packet_interval;

				pmt::pmt_t d_trigger_start_in_port;
				pmt::pmt_t d_trigger_stop_in_port;
				pmt::pmt_t d_packet_size_in_port;
				pmt::pmt_t d_burst_interval_in_port;
				pmt::pmt_t d_burst_duration_in_port;
				pmt::pmt_t d_packet_interval_in_port;
				pmt::pmt_t d_pdu_out_port;
				pmt::pmt_t d_request_out_port;

				void run();

			public:
			vbr_transmitter_impl(bool use_acks,
								 content_t content_type,
								 int constant_value,
								 int sequential_minimum,
								 int sequential_maximum,
								 const char *logfile);
			~vbr_transmitter_impl();

			void handle_trigger_start(pmt::pmt_t msg);

			void handle_trigger_stop(pmt::pmt_t msg);

			void set_trigger(bool *trigger, pmt::pmt_t msg);

			void set_packet_size(pmt::pmt_t msg);

			void set_burst_interval(pmt::pmt_t msg);

			void set_burst_duration(pmt::pmt_t msg);

			void set_packet_interval(pmt::pmt_t msg);

			void fill_payload(uint8_t *__payload, uint32_t __size);

			void open_logfile(const char *__filename);

			bool start();

			bool stop();
	};

  } // namespace trafficgen
} // namespace gr

#endif /* INCLUDED_TRAFFICGEN_VBR_TRANSMITTER_IMPL_H */

