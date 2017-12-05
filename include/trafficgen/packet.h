#ifndef INCLUDED_TRAFFICGEN_PACKET_H
#define INCLUDED_TRAFFICGEN_PACKET_H

#include <trafficgen/api.h>
#include <pmt/pmt.h>
#include <cstdint>

namespace gr {
	namespace trafficgen {

		#define PACKET_HEADER 0b00101010
		#define PACKET_PREFIX_ELEMENTS 4
		#define PACKET_PREFIX_SIZE (4 * sizeof(uint32_t))	// Bytes before payload
		#define PACKET_HEADERS_SIZE (5 * sizeof(uint32_t))	// Total bytes of headers on packet

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
		 * packet[m..n] = payload (converted from uint8_t to uint32_t) X B
		 * packet[n+1] = crc32 			4 B
		 * total of 20 B of headers
		 */

		class TRAFFICGEN_API packet {

			private:
				uint32_t d_header;
				bool d_use_acks;
				uint32_t d_id;
				uint32_t d_payload_length;
				uint32_t d_message_length;	// in Bytes
				uint32_t d_vector_length;	// size of our uint32_t vector
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

				// /* Message length is calculated based on payload length plus headers */
				uint32_t get_message_length();

				// /* Payload length does not consider header overhead */
				uint32_t get_payload_length();

				// /* We must pass payload length to ease the copy into the struct later... */
				void set_payload(uint8_t *__payload, uint32_t __payload_length);

				void get_payload(uint8_t *__buffer);

				uint32_t get_crc32();

				void generate_next();

				void generate_next(uint8_t *payload);

				pmt::pmt_t get_blob();

				void print();

				packet_status_t check();
		};
	} // namespace trafficgen
} // namespace gr

#endif /* INCLUDED_TRAFFICGEN_PACKET_H */
