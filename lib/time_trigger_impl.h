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

#ifndef INCLUDED_TRAFFICGEN_TIME_TRIGGER_IMPL_H
#define INCLUDED_TRAFFICGEN_TIME_TRIGGER_IMPL_H

#include <trafficgen/time_trigger.h>

namespace gr {

	namespace trafficgen {

	class time_trigger_impl : public time_trigger {

		private:
			boost::shared_ptr<gr::thread::thread> d_thread;
			bool d_finished;
			bool d_state;
			float d_active_time;
			float d_inactive_time;
			pmt::pmt_t d_msg;

			void run();

		public:
			time_trigger_impl(bool initial_state, float active_time, float inactive_time);
			~time_trigger_impl();

			void set_active_time(float active_time){ d_active_time = active_time; }

			void set_inactive_time(float inactive_time){ d_inactive_time = inactive_time; }
			
			bool start();
			bool stop();
		};
	} // namespace trafficgen
} // namespace gr

#endif /* INCLUDED_TRAFFICGEN_TIME_TRIGGER_IMPL_H */

