#ifndef INCLUDED_TRAFFICGEN_GENERATOR_CONSTANT_H
#define INCLUDED_TRAFFICGEN_GENERATOR_CONSTANT_H

#include <trafficgen/api.h>
#include <gnuradio/block.h>
#include <trafficgen/common.h>

namespace gr {
	namespace trafficgen {

		class TRAFFICGEN_API generator_constant : virtual public gr::block {

			public:
				typedef boost::shared_ptr<generator_constant> sptr;

				virtual void set_value(double value) = 0;

				static sptr make(vbr_ports_t vbr_port, double value);
		};
	} // namespace trafficgen
} // namespace gr

#endif /* INCLUDED_TRAFFICGEN_GENERATOR_CONSTANT_H */
