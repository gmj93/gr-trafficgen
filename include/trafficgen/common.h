#ifndef INCLUDED_TRAFFICGEN_COMMON_H
#define INCLUDED_TRAFFICGEN_COMMON_H

#include <trafficgen/api.h>

typedef enum {
	DIST_CONSTANT=0,
	DIST_UNIFORM=1,
	DIST_GAUSSIAN=2,
	DIST_POISSON=3,
	DIST_WEIBULL=4
} distribution_t;

typedef enum {
	CONTENT_CONSTANT=1,
	CONTENT_RANDOM=2,
	CONTENT_SEQUENTIAL=3
} content_t;

typedef enum {
	VBR_PORT_PACKET_SIZE,
	VBR_PORT_BURST_INTERVAL,
	VBR_PORT_REQUEST_BURST_DURATION,
	VBR_PORT_REQUEST_PACKET_INTERVAL
} vbr_ports_t;

#define MP_TRIGGER_START_IN   "Trigger Start"
#define MP_TRIGGER_STOP_IN    "Trigger Stop"
#define MP_PACKET_SIZE_IN     "Packet Size"
#define MP_BURST_INTERVAL_IN  "Burst Interval"
#define MP_BURST_DURATION_IN  "Burst Duration"
#define MP_PACKET_INTERVAL_IN "Packet Interval"
#define MP_PDU_OUT            "PDU"
#define MP_REQUEST_OUT        "Request"
#define MP_REQUEST_IN         "Request"
#define MP_GENERATOR_OUT      "Value"

namespace gr {
	namespace trafficgen {

		TRAFFICGEN_API uint64_t calculate_timestamp();

	} // namespace trafficgen
} // namespace gr

#endif /* INCLUDED_TRAFFICGEN_COMMON_H */

