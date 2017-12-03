#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include <trafficgen/packet.h>
#include <trafficgen/digital_crc32.h>
#include <iostream>
#include <cstring>

namespace gr {
	namespace trafficgen {

		packet::packet(uint32_t header,
					   bool use_acks,
					   uint32_t id,
					   uint32_t message_length)
			: d_header(header),
			  d_use_acks(use_acks),
			  d_id(id),
			  d_message_length(message_length){

		  	d_vector_length = ceil(d_message_length / sizeof(uint32_t));
		  	d_payload_length = (d_message_length - (5 * sizeof(uint32_t)));		// 5 fields (headers and crc)

		  	d_packet = new uint32_t[d_vector_length];
		  	d_packet[0] = d_header;
		  	d_packet[1] = d_use_acks ? 1 : 0;
		  	d_packet[2] = d_id;
		  	d_packet[3] = d_payload_length;
		}

		packet::~packet(){}

		void packet::set_header(uint32_t __header){

			d_header = __header;
			d_packet[0] = __header;
		}

		uint32_t packet::get_header(){

			return d_header;
		}

		void packet::set_use_acks(bool __use_acks){

			d_use_acks = __use_acks;
			d_packet[1] = __use_acks ? 1 : 0;
		}

		bool packet::get_use_acks(){

			return d_use_acks;
		}

		void packet::set_id(uint32_t __id){

			d_id = __id;
			d_packet[2] = __id;
		}

		uint32_t packet::get_id(){

			return d_id;
		}

		uint32_t packet::get_message_length(){

			return d_message_length;
		}

		uint32_t packet::get_payload_length(){

			return d_payload_length;
		}

		void packet::set_payload(uint8_t *__payload, uint32_t __payload_length){

			// could throw an error, but will just cut payload
			uint32_t amount_to_copy = (__payload_length > d_payload_length) ? d_payload_length : __payload_length;

			std::memcpy(&d_packet[4], __payload, amount_to_copy);	// put payload after headers
		}

		void packet::get_payload(uint8_t *__buffer){

			std::memcpy(__buffer, &d_packet[4], d_payload_length);	// payload is after headers
		}

		// could have made this more direct, but let's make things work first
		uint32_t packet::get_crc32(){

			uint32_t crc_section_size = d_message_length - 4;
			uint8_t *crc_section = new uint8_t[crc_section_size];

			memcpy(crc_section, d_packet, crc_section_size);

			d_crc32 = crc32(crc_section, crc_section_size);
			d_packet[d_vector_length - 1] = d_crc32;

			return d_crc32;
		}

		void packet::generate_next(){
			
		}

		void packet::generate_next(uint8_t *__payload){

		// 	// TODO, increment ID and does everything else necessary to generate another 
		// 	// packet faster ignoring previous packet's definitions. Call generate_next() then
		// 	// get_pmt_blob() or get_stream() to get a copy of the new packet...

			// set_payload(__payload, d_payload_length);
		}

		// pmt::pmt_t packet::to_pmt(){

		// 	pmt::pmt_t packet = pmt::make_dict();

		// 	pmt::pmt_t headers = pmt::make_dict();	// headers, use_acks, id, packet_length
		// 	pmt::pmt_t payload;
		// 	pmt::pmt_t crc32;

		// 	headers = pmt::dict_add(headers, pmt::intern("header"), pmt::from_long(d_header));
		// 	headers = pmt::dict_add(headers, pmt::intern("use_acks"), pmt::from_bool(d_use_acks));
		// 	headers = pmt::dict_add(headers, pmt::intern("id"), pmt::from_long(d_id));
		// 	headers = pmt::dict_add(headers, pmt::intern("payload_length"), pmt::from_long(d_payload_length));

		// 	packet = pmt::dict_add(packet, pmt::intern("headers"), headers);

		// 	if (d_payload_length){

		// 		 payload = pmt::make_blob(d_payload, d_payload_length);
		// 		 packet = pmt::dict_add(packet, pmt::intern("payload"), payload);
		// 	}

		// 	// TODO calculate crc32 and then store in packet

		// 	pmt::print(packet);

		// 	return packet;
		// }
	} /* namespace trafficgen */
} /* namespace gr */