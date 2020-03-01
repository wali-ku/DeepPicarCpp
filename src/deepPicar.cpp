#include <deepPicar.hpp>

DeepPicar::DeepPicar (InputFactory *input_obj, TensorFactory *tf_obj,
		      ClockFactory *clock_obj, LogFactory *log_obj,
		      RtgFactory *rtg_obj, int frame_limit):
	input (input_obj), tf (tf_obj), clock (clock_obj),
	log (log_obj), rtgang (rtg_obj), frames_to_process (frame_limit)
{
	/** Warmup Tensor. */
	tf->process (input->get_next_frame ());
}

DeepPicar::~DeepPicar ()
{
	delete tf;
	delete log;
	delete clock;
	delete input;
	delete rtgang;
}

void DeepPicar::run ()
{
	int frame_number = 0;
	timepoint job_start_time;
	float inference_time, response_time;
	float output_angle_rad, output_angle_deg;

	/**
	 * Synchronize if running as a virtual gang and then start the
	 * DNN inference loop.
	 */
	rtgang->sync ();
	clock->mark_periodic_start ();

	while (1) {
#ifdef RTG_SYNCH_DEBUG
		if (rtgang->leader)
			debug_log_ftrace ("gid=%d: job_start\n", rtgang->gid);
#endif

		/** Record the job start time. */
		job_start_time = clock->get_timestamp ();
		output_angle_rad = tf->process (input->get_next_frame ());

		/** Mark job as finished and calculate job duration. */
		inference_time = clock->get_job_duration (job_start_time,
							response_time);

		/** Record stats for this job in the output log. */
		output_angle_deg = 180.0 * output_angle_rad / M_PI;
		log->record (frame_number, inference_time, response_time,
				output_angle_deg);

		/**
		  * Exit the inference loop if the required number of
		  * image-frames have been processed.
		  */
		if (++frame_number >= frames_to_process)
			break;

		/** Sleep till it is time for the next job. */
		clock->sleep_till_next_job ();
	}

	cout << "Execution Complete!" << endl;

	return;
}
