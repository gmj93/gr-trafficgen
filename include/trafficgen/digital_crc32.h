#ifndef INCLUDED_TRAFFICGEN_DIGITAL_CRC32_H
#define INCLUDED_TRAFFICGEN_DIGITAL_CRC32_H

#include <trafficgen/api.h>

namespace gr {
	namespace trafficgen {

		// 	class TRAFFICGEN_API digital_crc32 {
		// public:
		//   digital_crc32();
		//   ~digital_crc32();
		// private:
		// };

	    TRAFFICGEN_API unsigned int 
    		update_crc32(unsigned int crc, const unsigned char *buf, size_t len);

    	TRAFFICGEN_API unsigned int 
    		update_crc32(unsigned int crc, const std::string buf);

    	TRAFFICGEN_API unsigned int 
    		crc32(const unsigned char *buf, size_t len);

    	TRAFFICGEN_API unsigned int 
    		crc32(const std::string buf);

	} // namespace trafficgen
} // namespace gr

#endif /* INCLUDED_TRAFFICGEN_DIGITAL_CRC32_H */

