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
			: gr::block("receiver", gr::io_signature::make(0, 0, 0), gr::io_signature::make(0, 0, 0)){

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

			// TODO store statistics

			return block::stop();

		}

		void receiver_impl::open_logfile(const char *__filename){

			d_logfile.open(__filename, std::ios::out | std::ios::app);

			if (!d_logfile.is_open()){

				throw std::runtime_error("Unable to open file");
			}
		}

		void receiver_impl::handle_incoming_packet(pmt::pmt_t __packet){

			pmt::print(__packet);
		}
	} /* namespace trafficgen */
} /* namespace gr */

