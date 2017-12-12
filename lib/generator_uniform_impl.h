/* -*- c++ -*- */
/* 
 * Copyright 2017 Gilson Miranda Jr <junior.kdm@gmail.com>
 * Copyright 2017 Matheus H. N. Nunes <mhnnunes@dcc.ufmg.br>
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_TRAFFICGEN_GENERATOR_UNIFORM_IMPL_H
#define INCLUDED_TRAFFICGEN_GENERATOR_UNIFORM_IMPL_H

#include <trafficgen/generator_uniform.h>
#include <boost/random/variate_generator.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real.hpp>

namespace gr {
	namespace trafficgen {

		class generator_uniform_impl : public generator_uniform {
			private:
				vbr_ports_t d_vbr_port;
				double d_minimum;
				double d_maximum;
				double d_multiplier;
				pmt::pmt_t d_msg;
				pmt::pmt_t d_request_in_port;
				pmt::pmt_t d_value_out_port;

				boost::mt19937 d_rng;

				boost::shared_ptr<
					boost::variate_generator <
						boost::mt19937, boost::uniform_real<>
					>
				> d_generator;

			public:
				generator_uniform_impl(vbr_ports_t vbr_port,
									   double minimum,
									   double maximum,
									   double multiplier);

				~generator_uniform_impl();

				void publish_value(pmt::pmt_t msg);

				bool start();

				bool stop();
		};
	} // namespace trafficgen
} // namespace gr

#endif /* INCLUDED_TRAFFICGEN_GENERATOR_UNIFORM_IMPL_H */

