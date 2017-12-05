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

		  	d_vector_length  = ceil(d_message_length / sizeof(uint32_t));
		  	d_payload_length = (d_message_length - PACKET_HEADERS_SIZE);

		  	d_packet         = new uint32_t[d_vector_length];
		  	d_packet[0]      = d_header;
		  	d_packet[1]      = d_use_acks ? 1 : 0;
		  	d_packet[2]      = d_id;
		  	d_packet[3]      = d_payload_length;
		}

		packet::packet(pmt::pmt_t __packet){

			if (!pmt::is_blob(__packet)){

				throw std::runtime_error("Handles only blobs for now...");
			}

			d_packet           = (uint32_t *) pmt::blob_data(__packet);
			d_message_length   = pmt::blob_length(__packet);
			d_vector_length    = ceil(d_message_length / sizeof(uint32_t));
			d_header           = d_packet[0];
			d_use_acks         = d_packet[1] ? true : false;
			d_id               = d_packet[2];
			d_payload_length   = d_packet[3];
			d_crc32            = d_packet[d_vector_length - 1];
		}

		packet::~packet(){}

		void packet::set_header(uint32_t __header){

			d_header    = __header;
			d_packet[0] = __header;
			d_crc32     = 0;		// Invalidate current CRC
		}

		uint32_t packet::get_header(){

			return d_header;
		}

		void packet::set_use_acks(bool __use_acks){

			d_use_acks  = __use_acks;
			d_packet[1] = __use_acks ? 1 : 0;
			d_crc32     = 0;
		}

		bool packet::get_use_acks(){

			return d_use_acks;
		}

		void packet::set_id(uint32_t __id){

			d_id        = __id;
			d_packet[2] = __id;
			d_crc32     = 0;
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

			std::memcpy(&d_packet[PACKET_PREFIX_ELEMENTS], __payload, amount_to_copy);	// put payload after headers

			d_crc32 = 0;
		}

		void packet::get_payload(uint8_t *__buffer){

			std::memcpy(__buffer, &d_packet[PACKET_PREFIX_ELEMENTS], d_payload_length * sizeof(uint8_t));	// payload is after headers
		}

		void packet::set_crc32(uint32_t __crc){

			d_crc32                       = __crc;
			d_packet[d_vector_length - 1] = __crc;
		}

		uint32_t packet::get_crc32(){

			return d_crc32;
		}

		uint32_t packet::calculate_crc32(){

			uint32_t crc_section_size = d_message_length - 4;
			uint8_t *crc_section = new uint8_t[crc_section_size];

			memcpy(crc_section, d_packet, crc_section_size);

			uint32_t crc = crc32(crc_section, crc_section_size);
			
			return crc;
		}

		void packet::generate_next(){
			
			set_id(++d_id);
			set_crc32(calculate_crc32());
		}

		void packet::generate_next(uint8_t *__payload){

			set_payload(__payload, d_payload_length);
			generate_next();
		}

		pmt::pmt_t packet::get_blob(){

			if (get_crc32() == 0){		// Just to make sure we won't return an erroneous CRC

				set_crc32(calculate_crc32());	
			}
			
			return pmt::make_blob(d_packet, d_message_length);
		}

		void packet::print(){

			std::cout << "-----------------------------------------" << std::endl << std::flush;
			std::cout << "Header: " << d_header << std::endl << std::flush;
			std::cout << "ACKs: " << (d_use_acks ? "Enabled" : "Disabled") << std::endl << std::flush;
			std::cout << "ID:" << d_id << std::endl << std::flush;
			std::cout << "Payload Size: " << d_payload_length << std::endl << std::flush;
			std::cout << "Total Size: " << d_message_length << std::endl << std::flush;
			std::cout << "CRC32: " << d_crc32 << std::endl << std::flush;
			std::cout << "Payload: [";

			if (d_payload_length){

				uint8_t *payload = reinterpret_cast<uint8_t *>(d_packet);

				for (uint32_t i = PACKET_PREFIX_SIZE; i < d_payload_length; i++){

					std::cout << (int)payload[i] << ' ';
				}
			}

			std::cout << "]\n" << std::endl << std::flush;
			std::cout << "-----------------------------------------" << std::endl << std::flush;
		}

		packet_status_t packet::check(){

			if (d_header != PACKET_HEADER){

				return PK_HEADER_ERR;

			} else if (d_crc32 != calculate_crc32()){

				return PK_CRC_ERR;
			}

			return PK_VALID;
		}
	} /* namespace trafficgen */
} /* namespace gr */