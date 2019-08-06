#include <clockFactory.hpp>

timepoint ClockFactory::get_timestamp ()
{
	return sys_clock::now ();
}

void ClockFactory::sleep_till_next_job (timepoint job_start_time)
{
	auto next_job_start_time = job_start_time + msecs (period_msec);

	if (sys_clock::now () < next_job_start_time)
		this_thread::sleep_until (next_job_start_time);

	return;
}

float ClockFactory::get_job_duration (timepoint job_start_time)
{
	float duration = chrono::duration_cast<msecs>(sys_clock::now () -
			job_start_time).count ();

	return duration;
}
