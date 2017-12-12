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

#ifndef INCLUDED_TRAFFICGEN_PACKET_H
#define INCLUDED_TRAFFICGEN_PACKET_H

#include <trafficgen/api.h>
#include <pmt/pmt.h>
#include <cstdint>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace gr {
	namespace trafficgen {

		#define PACKET_HEADER 0b00101010
		#define PACKET_MINIMUM_SIZE_B 32
		#define PACKET_PREFIX_ELEMENTS 6
		#define PACKET_PREFIX_SIZE (PACKET_PREFIX_ELEMENTS * sizeof(uint32_t))	// Bytes before payload
		#define PACKET_HEADERS_SIZE (7 * sizeof(uint32_t))	// Total bytes of headers on packet

		#define INDEX_HEAD 0
		#define INDEX_ACKS 1
		#define INDEX_PKID 2
		#define INDEX_PLEN 3
		#define INDEX_TSLO 4
		#define INDEX_TSHI 5

		typedef enum {
			PK_VALID,
			PK_HEADER_ERR,
			PK_CRC_ERR
		} packet_status_t;

		/* A packet here is a uint32_t vector, where:
		 * packet[0] = header			4 B
		 * packet[1] = use_acks			4 B
		 * packet[2] = id 				4 B
		 * packet[3] = payload_length	4 B
		 * packet[4] = timestamp Lo 	4 B
		 * packet[5] = timestamp Hi 	4 B
		 * packet[m..n] = payload (converted from uint8_t to uint32_t) X B
		 * packet[n+1] = crc32 			4 B
		 * total of 28 B of headers
		 */

		class TRAFFICGEN_API packet {

			private:
				uint32_t d_header;
				bool d_use_acks;
				uint32_t d_id;
				uint32_t d_payload_length;
				uint32_t d_message_length;	// in Bytes
				uint32_t d_vector_length;	// size of our uint32_t vector
				uint32_t d_timestamp_lo;
				uint32_t d_timestamp_hi;
				uint8_t *d_payload;
				uint32_t d_crc32;
				uint32_t *d_packet;

			private:
				void set_crc32(uint32_t __crc);

				uint32_t calculate_crc32();

			public:
				packet(uint32_t header,
					   bool use_acks,
					   uint32_t id,
					   uint32_t message_length);

				packet(pmt::pmt_t __packet);

				~packet();

				void set_header(uint32_t __header);

				uint32_t get_header();

				void set_use_acks(bool __use_acks);

				bool get_use_acks();

				void set_id(uint32_t __id);

				uint32_t get_id();

				/* Message length is calculated based on payload length plus headers */
				uint32_t get_message_length();

				/* Payload length does not consider header overhead */
				uint32_t get_payload_length();

				void set_payload(uint8_t *__payload, uint32_t __payload_length);

				void get_payload(uint8_t *__buffer);

				/* Returns packet's crc32 */
				uint32_t get_crc32();

				/* Returns packet's timestamp */
				uint64_t get_timestamp();

				/* Set packet's timestamp to now 
				 * **WARNING NOT AUTOMATICALLY SET ON CONSTRUCTOR!** 
				 */
				void set_timestamp();

				void generate_next();

				void generate_next(uint8_t *payload);

				pmt::pmt_t get_blob();

				void print();

				packet_status_t check();
		};
	} // namespace trafficgen
} // namespace gr

#endif /* INCLUDED_TRAFFICGEN_PACKET_H */

