#ifndef INCLUDED_TRAFFICGEN_RECEIVER_H
#define INCLUDED_TRAFFICGEN_RECEIVER_H

#include <trafficgen/api.h>
#include <gnuradio/block.h>

namespace gr {
	namespace trafficgen {

		class TRAFFICGEN_API receiver : virtual public gr::block {
	 		
	 		public:
				typedef boost::shared_ptr<receiver> sptr;

				virtual void open_logfile(const char *__filename) = 0;

				/*!
				 * \brief Return a shared_ptr to a new instance of trafficgen::receiver.
				 *
				 * To avoid accidental use of raw pointers, trafficgen::receiver's
				 * constructor is in a private implementation
				 * class. trafficgen::receiver::make is the public interface for
				 * creating new instances.
				 */
				static sptr make(const char *filename);
		};
	} // namespace trafficgen
} // namespace gr

#endif /* INCLUDED_TRAFFICGEN_RECEIVER_H */

