#ifndef INCLUDED_TRAFFICGEN_VBR_TRANSMITTER_H
#define INCLUDED_TRAFFICGEN_VBR_TRANSMITTER_H

#include <trafficgen/api.h>
#include <gnuradio/block.h>
#include <trafficgen/common.h>

namespace gr {
	namespace trafficgen {

		class TRAFFICGEN_API vbr_transmitter : virtual public gr::block	{
			public:
				typedef boost::shared_ptr<vbr_transmitter> sptr;

				virtual void open_logfile(const char *__filename) = 0;

				static sptr make(bool use_acks,
								 trafficgen_content_t content_type,
								 int constant_value,
								 int sequential_minimum,
								 int sequential_maximum,
								 const char *logfile);
		};
	} // namespace trafficgen
} // namespace gr

#endif /* INCLUDED_TRAFFICGEN_VBR_TRANSMITTER_H */

