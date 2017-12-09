#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include <trafficgen/common.h>
#include <trafficgen/packet.h>
#include <boost/chrono.hpp>
#include "vbr_transmitter_impl.h"

namespace gr {
	namespace trafficgen {

		vbr_transmitter::sptr
		vbr_transmitter::make(bool use_acks,
							  content_t content_type,
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
												   content_t content_type,
												   int constant_value,
												   int sequential_minimum,
												   int sequential_maximum,
												   const char *logfile)
			: gr::block("vbr_transmitter",gr::io_signature::make(0, 0, 0), gr::io_signature::make(0, 0, 0)),
			  d_finished(false),
			  d_trigger_start(true),
			  d_trigger_stop(true),
			  d_create_packets(true),
			  d_use_acks(use_acks),
			  d_content_type(content_type),
			  d_constant_value(constant_value),
			  d_sequential_minimum(sequential_minimum),
			  d_sequential_maximum(sequential_maximum),
			  d_packet_size(0),
			  d_burst_interval(0),
			  d_burst_duration(0),
			  d_packet_interval(0){

		  	open_logfile(logfile);

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

			uint64_t value = round(pmt::to_double(msg));

			// Packet must be at least 32 Bytes long...
			d_packet_size = (value < PACKET_MINIMUM_SIZE_B) ? PACKET_MINIMUM_SIZE_B : value;
		}

		void vbr_transmitter_impl::set_burst_interval(pmt::pmt_t msg){

			d_burst_interval = (uint64_t)(pmt::to_double(msg) * 1000.0);
		}

		void vbr_transmitter_impl::set_burst_duration(pmt::pmt_t msg){

			d_burst_duration = (uint64_t)(pmt::to_double(msg) * 1000.0);
		}

		void vbr_transmitter_impl::set_packet_interval(pmt::pmt_t msg){

			d_packet_interval = (uint64_t)(pmt::to_double(msg) * 1000.0);
		}

		void vbr_transmitter_impl::fill_payload(uint8_t *__payload, uint32_t __size){
			
			switch(d_content_type){

				case CONTENT_CONSTANT: {

					std::memset(__payload, d_constant_value, (__size * sizeof(uint8_t)));
				}
				break;

				case CONTENT_SEQUENTIAL: {

					uint32_t copied         = 0;
					uint32_t amount_to_copy = 0;
					uint32_t element_count  = (d_sequential_maximum - d_sequential_minimum + 1);
					uint8_t  elements[element_count];

					std::iota(elements, elements + element_count, d_sequential_minimum);

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

		void vbr_transmitter_impl::open_logfile(const char *__filename){

			d_logfile.open(__filename, std::ios::out | std::ios::app);

			if (!d_logfile.is_open()){

				throw std::runtime_error("Unable to open file");
			}
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

			boost::this_thread::disable_interruption di;

			uint32_t   id = 0;
			packet     *pk;
			uint32_t   payload_length;
			uint8_t    *payload;
			pmt::pmt_t blob_packet;
			uint32_t   stat_sent_packets       = 0;
			uint64_t   stat_sent_bytes         = 0;
			double     stat_average_throughput = 0;

			boost::posix_time::time_duration time_diff;
			boost::posix_time::ptime         start;
			boost::posix_time::ptime         stop;

			bool running_burst = false;
			boost::posix_time::time_duration burst_duration;
			boost::posix_time::ptime         burst_start;
			boost::posix_time::ptime 		 now;

			pmt::pmt_t request_packet_size     = pmt::from_long(VBR_PORT_PACKET_SIZE);
			pmt::pmt_t request_burst_interval  = pmt::from_long(VBR_PORT_BURST_INTERVAL);
			pmt::pmt_t request_burst_duration  = pmt::from_long(VBR_PORT_REQUEST_BURST_DURATION);
			pmt::pmt_t request_packet_interval = pmt::from_long(VBR_PORT_REQUEST_PACKET_INTERVAL);

			start = boost::posix_time::microsec_clock::local_time();

			while(!d_finished){

				message_port_pub(d_request_out_port, request_packet_size);
				message_port_pub(d_request_out_port, request_burst_interval);
				message_port_pub(d_request_out_port, request_burst_duration);
				message_port_pub(d_request_out_port, request_packet_interval);

				pk             = new packet(PACKET_HEADER, d_use_acks, id, d_packet_size);
				payload_length = pk->get_payload_length();
				payload        = new uint8_t[payload_length]; 

				fill_payload(payload, payload_length);

				pk->set_payload(payload, payload_length);

				burst_duration = boost::posix_time::microseconds(d_burst_duration);

				running_burst  = true;
				burst_start    = boost::posix_time::microsec_clock::local_time();

				do {

					pk->set_timestamp();
					blob_packet = pk->get_blob();
					message_port_pub(d_pdu_out_port, blob_packet);

					stat_sent_packets++;
					stat_sent_bytes += d_packet_size;

					std::cout << "[TX]["    << pk->get_id() 
							  << "][Size: " << pk->get_message_length() 
							  << " B]"       << std::endl << std::flush;

					boost::this_thread::sleep_for(boost::chrono::microseconds(d_packet_interval));

					now = boost::posix_time::microsec_clock::local_time();

					if ((now - burst_start) >= burst_duration){

						running_burst = false;	

					} else {

						pk->generate_next();
						id++;
					}

				} while (running_burst);

				boost::this_thread::sleep_for(boost::chrono::microseconds(d_burst_interval));

				if (!d_create_packets) {

					stop = boost::posix_time::microsec_clock::local_time();

					time_diff = stop - start;
					stat_average_throughput = (((double)stat_sent_bytes * 8.0) /
											   ((double)time_diff.total_milliseconds() / 1000.0));

					d_logfile << "\n---- Trigger interruption ----\n" << std::flush;
					d_logfile << "packets_sent;bytes_sent;tx_duration_ms;throughput\n" << std::flush;
					d_logfile << stat_sent_packets << ";" 
							  << stat_sent_bytes << ";" 
							  << time_diff.total_milliseconds() << ";"
							  << stat_average_throughput << "\n" 
							  << std::flush;

					boost::mutex::scoped_lock lock(d_mutex_condition);
					d_run_packet_creation.wait(lock);
					start = boost::posix_time::microsec_clock::local_time();

					stat_sent_packets       = 0;
					stat_sent_bytes         = 0;
					stat_average_throughput = 0;

				}

				delete pk;
				delete payload;

				if (d_finished){

					stop = boost::posix_time::microsec_clock::local_time();

					time_diff = stop - start;
					stat_average_throughput = (((double)stat_sent_bytes * 8.0) /
											   ((double)time_diff.total_milliseconds() / 1000.0));

					d_logfile << "\n---- Finished ----\n" << std::flush;
					d_logfile << "packets_sent;bytes_sent;tx_duration_ms;throughput\n" << std::flush;
					d_logfile << stat_sent_packets << ";" 
							  << stat_sent_bytes << ";" 
							  << time_diff.total_milliseconds() << ";"
							  << stat_average_throughput << "\n" 
							  << std::flush;

					d_logfile.close();

					return;
				}
			}
		}
	} /* namespace trafficgen */
} /* namespace gr */

