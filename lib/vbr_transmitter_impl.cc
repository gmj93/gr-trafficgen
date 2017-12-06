#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include <trafficgen/common.h>
#include <trafficgen/packet.h>
#include "vbr_transmitter_impl.h"

namespace gr {
	namespace trafficgen {

		vbr_transmitter::sptr
		vbr_transmitter::make(bool use_acks,
							  trafficgen_content_t content_type,
							  int constant_value,
							  int sequential_minimum,
							  int sequential_maximum,
							  const char *logfile){

			return gnuradio::get_initial_sptr (
				new vbr_transmitter_impl(
					use_acks,
					content_type,
					constant_value,
					sequential_minimum,
					sequential_maximum,
					logfile
				)
			);
		}

		vbr_transmitter_impl::vbr_transmitter_impl(bool use_acks,
												   trafficgen_content_t content_type,
												   int constant_value,
												   int sequential_minimum,
												   int sequential_maximum,
												   const char *logfile)
			: gr::block("vbr_transmitter",gr::io_signature::make(0, 0, 0), gr::io_signature::make(0, 0, 0)),
			  d_use_acks(use_acks),
			  d_content_type(content_type),
			  d_constant_value(constant_value),
			  d_sequential_minimum(sequential_minimum),
			  d_sequential_maximum(sequential_maximum),
			  d_packet_size(0),
			  d_burst_interval(0),
			  d_burst_duration(0),
			  d_packet_interval(0){

		  	d_trigger_start_in_port   = pmt::mp(MP_TRIGGER_START_IN);
			d_trigger_stop_in_port    = pmt::mp(MP_TRIGGER_STOP_IN);
			d_packet_size_in_port     = pmt::mp(MP_PACKET_SIZE_IN);
			d_burst_interval_in_port  = pmt::mp(MP_BURST_INTERVAL_IN);
			d_burst_duration_in_port  = pmt::mp(MP_BURST_DURATION_IN);
			d_packet_interval_in_port = pmt::mp(MP_PACKET_INTERVAL_IN);
			d_pdu_out_port            = pmt::mp(MP_PDU_OUT);
			d_request_out_port        = pmt::mp(MP_REQUEST_OUT);

			message_port_register_in(d_trigger_start_in_port);
			message_port_register_in(d_trigger_stop_in_port);
			message_port_register_in(d_packet_size_in_port);
			message_port_register_in(d_burst_interval_in_port);
			message_port_register_in(d_burst_duration_in_port);
			message_port_register_in(d_packet_interval_in_port);
			message_port_register_out(d_pdu_out_port);
			message_port_register_out(d_request_out_port);

			// Trigger Start
			set_msg_handler(d_trigger_start_in_port,
							boost::bind(&vbr_transmitter_impl::handle_trigger_start, this, _1));

			// Trigger Stop
			set_msg_handler(d_trigger_stop_in_port,
							boost::bind(&vbr_transmitter_impl::handle_trigger_stop, this, _1));

			// Packet Size
			set_msg_handler(d_packet_size_in_port,
							boost::bind(&vbr_transmitter_impl::set_packet_size, this, _1));

			// Burst Interval
			set_msg_handler(d_burst_interval_in_port,
							boost::bind(&vbr_transmitter_impl::set_burst_interval, this, _1));

			// Burst Duration
			set_msg_handler(d_burst_duration_in_port,
							boost::bind(&vbr_transmitter_impl::set_burst_duration, this, _1));

			// Packet Interval
			set_msg_handler(d_packet_interval_in_port,
							boost::bind(&vbr_transmitter_impl::set_packet_interval, this, _1));
		}

		vbr_transmitter_impl::~vbr_transmitter_impl(){}

		void vbr_transmitter_impl::handle_trigger_start(pmt::pmt_t msg){

			set_trigger(&d_trigger_start, msg);
		}

		void vbr_transmitter_impl::handle_trigger_stop(pmt::pmt_t msg){

			set_trigger(&d_trigger_stop, msg);
		}

		void vbr_transmitter_impl::set_trigger(bool *trigger, pmt::pmt_t msg){

			*trigger = (msg == pmt::PMT_T ? true : false);

			if (d_trigger_start && !d_trigger_stop){

				d_run_packet_creation.notify_one();
				d_create_packets = true;

			} else {

				d_create_packets = false;
			}
		}

		void vbr_transmitter_impl::set_packet_size(pmt::pmt_t msg){

			double value = round(pmt::to_double(msg));

			// Packet must be at least 32 Bytes long...
			d_packet_size = (value < PACKET_MINIMUM_SIZE_B) ? PACKET_MINIMUM_SIZE_B : value;
		}

		void vbr_transmitter_impl::set_burst_interval(pmt::pmt_t msg){

			d_burst_interval = pmt::to_double(msg);
		}

		void vbr_transmitter_impl::set_burst_duration(pmt::pmt_t msg){

			d_burst_duration = pmt::to_double(msg);
		}

		void vbr_transmitter_impl::set_packet_interval(pmt::pmt_t msg){

			d_packet_interval = pmt::to_double(msg);	
		}

		bool vbr_transmitter_impl::start(){

			d_finished = false;
			d_thread = boost::shared_ptr<gr::thread::thread> 
			(new gr::thread::thread(boost::bind(&vbr_transmitter_impl::run, this)));

			return block::start();
		}

		bool vbr_transmitter_impl::stop(){

			d_finished = true;
			d_run_packet_creation.notify_one();
			d_thread->interrupt();
			d_thread->join();

			std::cout << std::flush;

			return block::stop();
		}

		void vbr_transmitter_impl::run(){

			pmt::pmt_t request_packet_size     = pmt::from_long(VBR_PORT_PACKET_SIZE);
			pmt::pmt_t request_burst_interval  = pmt::from_long(VBR_PORT_BURST_INTERVAL);
			pmt::pmt_t request_burst_duration  = pmt::from_long(VBR_PORT_REQUEST_BURST_DURATION);
			pmt::pmt_t request_packet_interval = pmt::from_long(VBR_PORT_REQUEST_PACKET_INTERVAL);

			while(!d_finished){

				message_port_pub(d_request_out_port, request_packet_size);
				message_port_pub(d_request_out_port, request_burst_interval);
				message_port_pub(d_request_out_port, request_burst_duration);
				message_port_pub(d_request_out_port, request_packet_interval);

				std::cout << d_packet_size << ";"
						  << d_burst_interval << ";"
						  << d_burst_duration << ";"
						  << d_packet_interval << std::endl;

				boost::this_thread::sleep(boost::posix_time::milliseconds(100));

				if (d_finished){

					return;
				}
			}
		}
	} /* namespace trafficgen */
} /* namespace gr */

