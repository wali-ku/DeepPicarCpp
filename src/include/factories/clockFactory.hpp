/**
 * ClockFactory Class Declaration File.
 *
 * This class implements time-keeping functionality for the DeepPicar.
 */
#ifndef __CLOCK_FACTORY_HPP__
#define __CLOCK_FACTORY_HPP__

#include <chrono>
#include <thread>

using namespace std;

using msecs = chrono::milliseconds;
using sys_clock = chrono::system_clock;
using timepoint = sys_clock::time_point;

class ClockFactory
{
	int		period_msec;

	public:
		/**
		 * Default Constructor. Should not be used.
		 */
		ClockFactory ();

		/**
		 * Class Constructor. Record period of inference loop from
		 * the user in milli-second granularity.
		 */
		ClockFactory (int period): period_msec (period) {}

		/**
		 * Time-stamp generator. Return current system-clock timestamp
		 * to caller.
		 */
		timepoint get_timestamp ();

		/**
		 * Job duration calculator. Return duration (in milli-seconds)
		 * between current system-clock time and job_start_time.
		 */
		float get_job_duration (timepoint job_start_time);

		/**
		 * Helper function for self-suspension. Sleep till it is time
		 * for the next job using job_start_time value and the
		 * inference period.
		 */
		void sleep_till_next_job (timepoint job_start_time);
};

#endif /* __CLOCK_FACTORY_HPP__ */
