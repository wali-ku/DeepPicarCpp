#include <logFactory.hpp>

LogFactory::LogFactory (bool console_output):
	print_to_console (console_output)
{
	string log_file_name;
	stringstream ss;

	auto t = time (nullptr);
	auto tm = *localtime (&t);
	ss << put_time (&tm, "run__%d-%m-%Y_%H-%M-%S.log");
	log_file_name = ss.str ();

	log_file.open (log_file_name);
	record_header ("Frame Number", "Job Duration (msec)", "Output (degrees)");
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

void LogFactory::record_header (string f1, string f2, string f3)
{
	stringstream ss;

	ss	<< setw (20) << f1 << "| "
		<< setw (20) << f2 << "| "
		<< setw (20) << f3 << endl;

	log_file << ss.str ();
	print_horizontal_line ('-');

	return;
}

void LogFactory::record (int frame_number, float duration, float output_angle)
{
	stringstream ss;
	time_log.push_back (duration);

	ss	<< setw (20) << frame_number 			  << "| "
		<< setw (20) << setprecision (5) << duration	  << "| "
		<< setw (20) << setprecision (5) << output_angle << endl;

	log_file << ss.str ();

	if (print_to_console)
		cout << ss.str ();

	return;
}

void LogFactory::print_stats ()
{
	stringstream ss;
	float mean_inference_time = accumulate (time_log.begin (),
			time_log.end (), 0.0) / time_log.size ();

	print_horizontal_line ('=');

	ss << "Mean Inference Time: " << setprecision (5)
		<< mean_inference_time << endl;

	log_file << ss.str ();
	cout << ss.str ();

	return;
}
