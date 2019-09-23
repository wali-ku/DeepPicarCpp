#include <logFactory.hpp>

LogFactory::LogFactory (bool console_output):
	print_to_console (console_output), num_of_records (0),
	cum_compute_time (0), cum_response_time (0)
{
	string log_file_name;
	stringstream ss;

	auto t = time (nullptr);
	auto tm = *localtime (&t);
	ss << put_time (&tm, "run__%d-%m-%Y_%H-%M-%S.log");
	srand (reinterpret_cast<uintptr_t>(&t));
	log_file_name = ss.str () + "_" + to_string (rand () % 1000);

	log_file.open (log_file_name);
	record_header ("Frame Number", "Job Duration (msec)",
			"Response Time (msec)", "Output (degrees)");
}

LogFactory::~LogFactory ()
{
	print_stats ();
	log_file.close ();
}

void LogFactory::print_horizontal_line (char symbol)
{
	string horizontal_rule (65, symbol);

	log_file << horizontal_rule << endl;

	if (print_to_console)
		cout << horizontal_rule << endl;

	return;
}

void LogFactory::record_header (string f1, string f2, string f3, string f4)
{
	stringstream ss;

	ss	<< setw (20) << f1 << "| "
		<< setw (20) << f2 << "| "
		<< setw (20) << f3 << "| "
		<< setw (20) << f4 << endl;

	log_file << ss.str ();
	print_horizontal_line ('-');

	return;
}

void LogFactory::record (int frame_number, float compute_time,
		float response_time, float output_angle)
{
	stringstream ss;
	float compute_msec = compute_time / 1000;
	float response_msec = response_time / 1000;

	cum_compute_time += compute_msec;
	cum_response_time += response_msec;
	num_of_records++;

	ss	<< setw (20) << frame_number 			  << "| "
		<< setw (20) << setprecision (5) << compute_msec  << "| "
		<< setw (20) << setprecision (5) << response_msec << "| "
		<< setw (20) << setprecision (5) << output_angle  << endl;

	log_file << ss.str ();

	if (print_to_console)
		cout << ss.str ();

	return;
}

void LogFactory::print_stats ()
{
	stringstream ss;
	float mean_inference_time = cum_compute_time / num_of_records;
	float mean_response_time = cum_response_time / num_of_records;

	print_horizontal_line ('=');

	ss << "Mean Inference Time: " << setprecision (5)
	   << mean_inference_time << " msec" << endl
	   << "Mean Response Time : " << setprecision (5)
	   << mean_response_time << " msec" << endl;

	log_file << ss.str ();
	cout << ss.str ();

	return;
}
