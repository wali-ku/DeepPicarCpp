/**
 * LogFactory Class Declaration File.
 *
 * This class is responsible for logging the execution statistics of DeepPicar.
 * The statistics are stored in an auto-generated log file. They can also be
 * copied to console as per the class initialization.
 */
#ifndef __LOG_FACTORY_HPP__
#define __LOG_FACTORY_HPP__

#include <iostream>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>

using namespace std;

class LogFactory
{
	vector<float>	time_log;
	ofstream	log_file;
	bool		print_to_console;

	void print_stats ();
	void print_horizontal_line (char symbol);
	void record_header (string f1, string f2, string f3);

	public:
		/**
		 * Default Constructor. Should not be used.
		 */
		LogFactory ();

		/**
		 * Class Constructor. Open and timestamp the log file for
		 * storing the execution statistics. The input parameter
		 * determines if the statistics should be copied to stdout.
		 */
		LogFactory (bool console_output);

		/**
		 * Class Destructor. Print the final execution statistics and
		 * close the log file.
		 */
		~LogFactory ();

		/**
		 * Logging helper function. Record the given statistics into
		 * the log file.
		 */
		void record (int frame_number, float duration,
			float output_angle);
};

#endif /* __LOG_FACTORY_HPP__ */
