#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "generator_constant_impl.h"
#include <trafficgen/common.h>

namespace gr {
  	namespace trafficgen {

  		generator_constant::sptr
  		generator_constant::make(trafficgen_vbr_ports_t vbr_port, double value){

  			return gnuradio::get_initial_sptr (new generator_constant_impl(vbr_port, value));
		}

		generator_constant_impl::generator_constant_impl(trafficgen_vbr_ports_t vbr_port, double value)
	  		: gr::block("generator_constant", gr::io_signature::make(0, 0, 0), gr::io_signature::make(0, 0, 0)),
	  		  d_vbr_port(vbr_port),
	  		  d_value(value){

	  		d_request_in_port = pmt::mp(MP_REQUEST_IN);
			d_value_out_port = pmt::mp(MP_GENERATOR_OUT);

			message_port_register_in(d_request_in_port);
			message_port_register_out(d_value_out_port);

			set_msg_handler(d_request_in_port,
							boost::bind(&generator_constant_impl::publish_value, this, _1));
		}

		generator_constant_impl::~generator_constant_impl(){}

		void generator_constant_impl::set_value(double value){

			d_value = value;
		}

		void generator_constant_impl::publish_value(pmt::pmt_t msg){

			if (pmt::to_long(msg) == d_vbr_port){

				message_port_pub(d_value_out_port, pmt::from_double(d_value));
			}
		}

		bool generator_constant_impl::start(){

			return block::start();
		}

		bool generator_constant_impl::stop(){

			return block::stop();
		}

  } /* namespace trafficgen */
} /* namespace gr */

