#ifndef INCLUDED_TRAFFICGEN_GENERATOR_CONSTANT_IMPL_H
#define INCLUDED_TRAFFICGEN_GENERATOR_CONSTANT_IMPL_H

#include <trafficgen/generator_constant.h>
#include <trafficgen/common.h>

namespace gr {
	namespace trafficgen {

		class generator_constant_impl : public generator_constant {

			private:
				trafficgen_vbr_ports_t d_vbr_port;
				double d_value;

				pmt::pmt_t d_msg;
				pmt::pmt_t d_request_in_port;
				pmt::pmt_t d_value_out_port;

			public:
				generator_constant_impl(trafficgen_vbr_ports_t vbr_port, double value);
				~generator_constant_impl();

				void set_value(double value);

				void publish_value(pmt::pmt_t msg);

				bool start();

				bool stop();
		};
	} // namespace trafficgen
} // namespace gr

#endif /* INCLUDED_TRAFFICGEN_GENERATOR_CONSTANT_IMPL_H */

