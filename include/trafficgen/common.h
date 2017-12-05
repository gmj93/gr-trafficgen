#ifndef INCLUDED_TRAFFICGEN_COMMON_H
#define INCLUDED_TRAFFICGEN_COMMON_H

#include <trafficgen/api.h>
#include <random>

typedef enum {
	DIST_UNIFORM=1,
	DIST_GAUSSIAN=2,
	DIST_POISSON=3,
	DIST_WEIBULL=4
} trafficgen_random_distribution_t;

typedef enum {
	CONTENT_CONSTANT=1,
	CONTENT_RANDOM=2,
	CONTENT_SEQUENTIAL=3
} trafficgen_content_t;

namespace gr {
	namespace trafficgen {

		TRAFFICGEN_API uint64_t calculate_timestamp();

	} // namespace trafficgen
} // namespace gr

#endif /* INCLUDED_TRAFFICGEN_COMMON_H */

