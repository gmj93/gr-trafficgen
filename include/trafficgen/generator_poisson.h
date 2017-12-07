#ifndef INCLUDED_TRAFFICGEN_GENERATOR_POISSON_H
#define INCLUDED_TRAFFICGEN_GENERATOR_POISSON_H

#include <trafficgen/api.h>
#include <gnuradio/block.h>
#include <trafficgen/common.h>

namespace gr {
	namespace trafficgen {
		
		class TRAFFICGEN_API generator_poisson : virtual public gr::block {

	 		public:
				typedef boost::shared_ptr<generator_poisson> sptr;

				static sptr make(vbr_ports_t vbr_port,
								 double mean,
								 double multiplier);
		};
	} // namespace trafficgen
} // namespace gr

#endif /* INCLUDED_TRAFFICGEN_GENERATOR_POISSON_H */

