#ifndef INCLUDED_TRAFFICGEN_RECEIVER_IMPL_H
#define INCLUDED_TRAFFICGEN_RECEIVER_IMPL_H

#include <trafficgen/receiver.h>
#include <iostream>
#include <fstream>

namespace gr {
	namespace trafficgen {

		class receiver_impl : public receiver {

		private:
			std::ofstream d_logfile;

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

