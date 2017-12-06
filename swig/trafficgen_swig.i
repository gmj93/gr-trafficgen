/* -*- c++ -*- */

#define TRAFFICGEN_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "trafficgen_swig_doc.i"

%{
#include "trafficgen/cbr_transmitter.h"
#include "trafficgen/time_trigger.h"
#include "trafficgen/user_trigger.h"
#include "trafficgen/common.h"
#include "trafficgen/packet.h"
#include "trafficgen/digital_crc32.h"
#include "trafficgen/receiver.h"
#include "trafficgen/vbr_transmitter.h"
#include "trafficgen/generator_uniform.h"
#include "trafficgen/generator_gaussian.h"
#include "trafficgen/generator_poisson.h"
#include "trafficgen/generator_weibull.h"
%}

%include "trafficgen/cbr_transmitter.h"
GR_SWIG_BLOCK_MAGIC2(trafficgen, cbr_transmitter);

%include "trafficgen/time_trigger.h"
GR_SWIG_BLOCK_MAGIC2(trafficgen, time_trigger);
%include "trafficgen/user_trigger.h"
GR_SWIG_BLOCK_MAGIC2(trafficgen, user_trigger);
%include "trafficgen/common.h"
%include "trafficgen/packet.h"
%include "trafficgen/digital_crc32.h"
%include "trafficgen/receiver.h"
GR_SWIG_BLOCK_MAGIC2(trafficgen, receiver);
%include "trafficgen/vbr_transmitter.h"
GR_SWIG_BLOCK_MAGIC2(trafficgen, vbr_transmitter);
%include "trafficgen/generator_uniform.h"
GR_SWIG_BLOCK_MAGIC2(trafficgen, generator_uniform);
%include "trafficgen/generator_gaussian.h"
GR_SWIG_BLOCK_MAGIC2(trafficgen, generator_gaussian);
%include "trafficgen/generator_poisson.h"
GR_SWIG_BLOCK_MAGIC2(trafficgen, generator_poisson);
%include "trafficgen/generator_weibull.h"
GR_SWIG_BLOCK_MAGIC2(trafficgen, generator_weibull);
