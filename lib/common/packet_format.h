#ifndef __PACKET_FORMAT_H__
#define __PACKET_FORMAT_H__

typedef struct {
	
	uint32_t header;
	uint32_t msglen;
	uint32_t crc;
	uint8_t *payload;
} tgenpacket_t;

#endif