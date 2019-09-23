#include <clockFactory.hpp>

timepoint ClockFactory::get_timestamp ()
{
	return sys_clock::now ();
}

void ClockFactory::mark_periodic_start ()
{
	period_start_time = get_timestamp ();

	return;
}

void ClockFactory::sleep_till_next_job ()
{
	auto next_job_start_time = period_start_time + msecs (period_msec);
	period_start_time = next_job_start_time;

	if (sys_clock::now () < next_job_start_time)
		this_thread::sleep_until (next_job_start_time);

	return;
}

float ClockFactory::get_job_duration (timepoint job_start_time, float& response_time)
{
	float duration = chrono::duration_cast<usecs>(sys_clock::now () -
			job_start_time).count ();

	response_time = chrono::duration_cast<usecs>(sys_clock::now () -
			period_start_time).count ();

	return duration;
}
