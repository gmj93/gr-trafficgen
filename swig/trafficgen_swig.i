/* -*- c++ -*- */

#define TRAFFICGEN_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "trafficgen_swig_doc.i"

%{
#include "trafficgen/receiver_b.h"
#include "trafficgen/cbr_transmitter.h"
#include "trafficgen/time_trigger.h"
#include "trafficgen/user_trigger.h"
%}


%include "trafficgen/receiver_b.h"
GR_SWIG_BLOCK_MAGIC2(trafficgen, receiver_b);
%include "trafficgen/cbr_transmitter.h"
GR_SWIG_BLOCK_MAGIC2(trafficgen, cbr_transmitter);

%include "trafficgen/time_trigger.h"
GR_SWIG_BLOCK_MAGIC2(trafficgen, time_trigger);
%include "trafficgen/user_trigger.h"
GR_SWIG_BLOCK_MAGIC2(trafficgen, user_trigger);
