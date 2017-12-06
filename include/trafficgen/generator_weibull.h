#ifndef INCLUDED_TRAFFICGEN_GENERATOR_WEIBULL_H
#define INCLUDED_TRAFFICGEN_GENERATOR_WEIBULL_H

#include <trafficgen/api.h>
#include <gnuradio/block.h>
#include <trafficgen/common.h>

namespace gr {
	namespace trafficgen {

		class TRAFFICGEN_API generator_weibull : virtual public gr::block {
			public:
				typedef boost::shared_ptr<generator_weibull> sptr;
				
				static sptr make(trafficgen_vbr_ports_t vbr_port,
								 double shape,
								 double scale,
								 double multiplier);
		};
	} // namespace trafficgen
} // namespace gr

#endif /* INCLUDED_TRAFFICGEN_GENERATOR_WEIBULL_H */

