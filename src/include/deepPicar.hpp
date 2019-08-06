/**
 * DeepPicar Class Declaration File.
 *
 * This file contains definition of DeepPicar class. The main autonomous
 * driving loop is implemented in this class.
 */
#ifndef __DEEP_PICAR_HPP__
#define __DEEP_PICAR_HPP__

#define _USE_MATH_DEFINES

#include <iostream>
#include <csignal>
#include <cmath>

#include <tensorFactory.hpp>
#include <clockFactory.hpp>
#include <inputFactory.hpp>
#include <logFactory.hpp>
#include <rtgFactory.hpp>

using namespace std;

class DeepPicar
{
	TensorFactory	*tf;
	LogFactory	*log;
	ClockFactory	*clock;
	InputFactory	*input;
	RtgFactory	*rtgang;
	int		frames_to_process;

	public:
		/**
		 * Default Constructor. Should not be invoked.
		 */
		DeepPicar ();

		/**
		 * Class Constructor. Receive all member object instances and
		 * required arguments from the caller.
		 */
		DeepPicar (InputFactory *input_obj, TensorFactory *tf_obj,
			ClockFactory *clock_obj, LogFactory *log,
			RtgFactory *rtg_obj, int frame_limit);

		/**
		 * Class Destructor. Destroy the member object instances and
		 * print the final execution statistics of DeepPicar.
		 */
		~DeepPicar ();

		/**
		 * Run the DeepPicar. This function contains the main inference
		 * loop of DeepPicar.
		 */
		void run ();
};

#endif /* __DEEP_PICAR_HPP__ */
