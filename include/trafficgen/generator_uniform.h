#ifndef INCLUDED_TRAFFICGEN_GENERATOR_UNIFORM_H
#define INCLUDED_TRAFFICGEN_GENERATOR_UNIFORM_H

#include <trafficgen/api.h>
#include <gnuradio/block.h>
#include <trafficgen/common.h>

namespace gr {
	namespace trafficgen {

		class TRAFFICGEN_API generator_uniform : virtual public gr::block {
			public:
				typedef boost::shared_ptr<generator_uniform> sptr;

				static sptr make(trafficgen_vbr_ports_t vbr_port,
								 double minimum,
								 double maximum,
								 double multiplier);
		};
	} // namespace trafficgen
} // namespace gr

#endif /* INCLUDED_TRAFFICGEN_GENERATOR_UNIFORM_H */
