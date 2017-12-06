#ifndef INCLUDED_TRAFFICGEN_GENERATOR_WEIBULL_IMPL_H
#define INCLUDED_TRAFFICGEN_GENERATOR_WEIBULL_IMPL_H

#include <trafficgen/generator_weibull.h>
#include <boost/random/variate_generator.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/weibull_distribution.hpp>

namespace gr {
	namespace trafficgen {

		class generator_weibull_impl : public generator_weibull {
			private:
				trafficgen_vbr_ports_t d_vbr_port;
				double d_shape;
				double d_scale;
				double d_multiplier;
				pmt::pmt_t d_msg;
				pmt::pmt_t d_request_in_port;
				pmt::pmt_t d_value_out_port;

				boost::mt19937 d_rng;

				boost::shared_ptr<
					boost::variate_generator <
						boost::mt19937, boost::random::weibull_distribution<>
					>
				> d_generator;

			public:
				generator_weibull_impl(trafficgen_vbr_ports_t vbr_port,
									   double shape,
									   double scale,
									   double multiplier);

				~generator_weibull_impl();

				void publish_value(pmt::pmt_t msg);

				bool start();

				bool stop();
		};
	} // namespace trafficgen
} // namespace gr

#endif /* INCLUDED_TRAFFICGEN_GENERATOR_WEIBULL_IMPL_H */
