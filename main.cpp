/**
 * Main Entry Point for DeepPicar Module.
 *
 * This file contains the main function for DeepPicar. It accepts command-line
 * arguments for instantiating appropriate helper objects and then creates
 * deepPicar object for performing inference on the given input video file.
 */

#include "deepPicar.hpp"

/**
 * Global DeepPicar Object. Must be instantiated later.
 */
DeepPicar 	*dcar;

/**
 * Value assigned to uninitialized strings. Used for error checking.
 */
string		UNINIT_STRING = "N/A";

/**
 * Value assigned to uninitialized integers. Used for error checking.
 */
int		UNINIT_INT = 0;

/**
 * handle_signal: Generic signal handler.
 *
 * Invoked in response to CTRL+C or CTRL+Z from the user. Prints final
 * statistics and exits the program.
 */
void handle_signal (int signum)
{
	cout << "[INTERRUPT] Signal Received: " << signum << endl;

	delete dcar;
	exit (signum);
}

void setup_signals ()
{
	signal (SIGINT, handle_signal);
	signal (SIGTSTP, handle_signal);

	return;
}

/**
 * print_usage: Display usage of the program on command-line.
 */
void print_usage (string exec_name)
{
	cout << "Usage: " << exec_name << " <options>" << endl;
	cout << endl;

	cout << "Options:" << endl;
	cout << "  -i <input>      Name of the input video file. Must be in \n";
	cout << "                  the same directory as the executable.\n";
	cout << "  -m <model>      Name of the tensor-flow model file. Must \n";
	cout << "                  in the executable directory.\n";
	cout << "  -l <limit>      Total number of frames to process.\n";
	cout << "  -f <fps>        Frames per second. This value controls \n";
	cout << "                  the period of execution for each job.\n";
	cout << "  -v <id>         Virtual gang ID value. Providing this \n";
	cout << "                  value signifies that this program is \n";
	cout << "                  running as part of a virtual gang.\n";
	cout << "  -r <mbps>       Allowed threshold of memory read ops \n";
	cout << "                  from co-running best-effort tasks.\n";
	cout << "  -w <mbps>       Allowed threshold of memory write ops \n";
	cout << "                  from co-running best-effort tasks.\n";
	cout << "  -o              Console output switch. If provided then \n";
	cout << "                  output log is printed to stdout.\n";
	cout << "  -h              Show this help message\n";
	cout << endl;

	cout << "Example:" << endl;
	cout << "  $ " << exec_name << " -i vid.avi -m mdl.pb -l 100 -f 30\n";
	cout << "                  Load video from file <vid.avi> and load \n";
	cout << "                  tensor-flow model from <mdl.pb>. Process \n";
	cout << "                  100 frames from the video at a rate of \n";
	cout << "                  1 frame / 33.3 msec (i.e., 30 Frames/sec)\n";
	cout << endl;

	exit (0);
}

/**
 * check_cmd_args: Command-line arguments checker.
 *
 * This function verifies that the user has supplied all the required
 * command-line arguments.
 */
void check_cmd_args (string exec_name, string vid, string mdl,
		int limit, int fps)

{
	if (vid != UNINIT_STRING && mdl != UNINIT_STRING &&
		limit != UNINIT_INT && fps != UNINIT_INT)
		return;

	if (vid == UNINIT_STRING)
		cerr << "Please provide a video file!\n";
	else if (mdl == UNINIT_STRING)
		cerr << "Please provide a model file!\n";
	else if (limit == UNINIT_INT)
		cerr << "Please specify number of frames to process!\n";
	else
		cerr << "Please specify frame rate!\n";

	print_usage (exec_name);

	return;
}

int main (int argc, char** argv)
{
	int opt;
	int period_msec;
	bool console_output = false;
	int frame_rate = UNINIT_INT;
	int frame_limit = UNINIT_INT;
	int read_bw_mbps = UNINIT_INT;
	int write_bw_mbps = UNINIT_INT;
	int virtual_gang_id = UNINIT_INT;
	string video_file = UNINIT_STRING;
	string model_name = UNINIT_STRING;

	LogFactory *log_obj;
	TensorFactory *tf_obj;
	ClockFactory *clock_obj;
	InputFactory *input_obj;
	RtgFactory *rtg_obj = NULL;

	/** Retrieve command line arguments. */
	while ((opt = getopt (argc, argv, "i:m:l:f:v:r:w:oh")) != -1) {
		switch (opt) {
			case 'i':
				video_file = optarg;
				break;
			case 'm':
				model_name = optarg;
				break;
			case 'l':
				frame_limit = stoi (optarg);
				break;
			case 'f':
				frame_rate = stoi (optarg);
				break;
			case 'v':
				virtual_gang_id = stoi (optarg);
				break;
			case 'r':
				read_bw_mbps = stoi (optarg);
				break;
			case 'w':
				write_bw_mbps = stoi (optarg);
				break;
			case 'o':
				console_output = true;
				break;
			default:
				print_usage (argv [0]);
		}

	}

	check_cmd_args (argv [0], video_file, model_name,
		frame_limit, frame_rate);

	/** Instantiate class objects. */
	period_msec = 1000 / frame_rate;
	tf_obj = new TensorFactory (model_name);
	clock_obj = new ClockFactory (period_msec);
	input_obj = new InputFactory (video_file);
	log_obj = new LogFactory (console_output);

	/**
	 * Instantiate RTG-Synch interface object if running as part of a
	 * virtual gang. Otherwise, leave the object NULL.
	 *
	 * Note that the user can leave the bandwidth thresholds blank. Since
	 * the uninitialized values of these variables is "0", the RTG-Synch
	 * library will interpret it as a direction to use default budget
	 * values for co-running best-effort tasks.
	 */
	if (virtual_gang_id != UNINIT_INT)
		rtg_obj = new RtgFactory (virtual_gang_id, read_bw_mbps,
				write_bw_mbps);

	dcar = new DeepPicar (input_obj, tf_obj, clock_obj, log_obj, rtg_obj,
			frame_limit);

	/** Start the autonomous driving experiment. */
	dcar->run ();
	delete dcar;

	return 0;
}
