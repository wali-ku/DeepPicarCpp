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
	if (rtgang) delete rtgang;
}

void DeepPicar::run ()
{
	int frame_number = 0;
	float inference_time;
	timepoint job_start_time;
	float output_angle_rad, output_angle_deg;

	while (1) {
		/** Record the job start time. */
		job_start_time = clock->get_timestamp ();

		/**
		 * Synchronize if running as a virtual gang and then perform
		 * DNN inference on the next image-frame from the input source.
		 */
		if (rtgang) rtgang->sync ();
		output_angle_rad = tf->process (input->get_next_frame ());

		/** Mark job as finished and calculate job duration. */
		inference_time = clock->get_job_duration (job_start_time);

		/** Record stats for this job in the output log. */
		output_angle_deg = 180.0 * output_angle_rad / M_PI;
		log->record (frame_number, inference_time, output_angle_deg);

		/**
		  * Exit the inference loop if the required number of
		  * image-frames have been processed.
		  */

		if (++frame_number >= frames_to_process)
			break;

		/** Sleep till it is time for the next job. */
		clock->sleep_till_next_job (job_start_time);
	}

	cout << "Execution Complete!" << endl;

	return;
}
