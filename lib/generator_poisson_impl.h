#ifndef INCLUDED_TRAFFICGEN_GENERATOR_POISSON_IMPL_H
#define INCLUDED_TRAFFICGEN_GENERATOR_POISSON_IMPL_H

#include <trafficgen/generator_poisson.h>
#include <trafficgen/common.h>
#include <boost/random/variate_generator.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/poisson_distribution.hpp>

namespace gr {
	namespace trafficgen {

		class generator_poisson_impl : public generator_poisson {
			private:
				vbr_ports_t d_vbr_port;
				double d_mean;
				double d_multiplier;
				pmt::pmt_t d_msg;
				pmt::pmt_t d_request_in_port;
				pmt::pmt_t d_value_out_port;

				boost::mt19937 d_rng;

				boost::shared_ptr<
					boost::variate_generator <
						boost::mt19937, boost::random::poisson_distribution<>
					>
				> d_generator;

			public:
				generator_poisson_impl(vbr_ports_t vbr_port,
									   double mean,
									   double multiplier);

				~generator_poisson_impl();

				void publish_value(pmt::pmt_t msg);

				bool start();

				bool stop();
		};
	} // namespace trafficgen
} // namespace gr

#endif /* INCLUDED_TRAFFICGEN_GENERATOR_POISSON_IMPL_H */

