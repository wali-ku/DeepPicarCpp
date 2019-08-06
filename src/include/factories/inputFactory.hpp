/**
 * InputFactory Class Declaration File.
 *
 * This class deals with handling the input source for the DeepPicar. It is
 * responsible for providing image-frames to the DeepPicar for inferencing.
 */
#ifndef __INPUT_FACTORY_HPP__
#define __INPUT_FACTORY_HPP__

#include <iostream>
#include <string>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <tensorflow/core/platform/env.h>

using namespace cv;
using namespace tensorflow;

class InputFactory
{
	VideoCapture	input_stream;

	public:
		/**
		 * Default Constructor. Should not be used.
		 */
		InputFactory ();

		/**
		 * Class Constructor. Use open-cv to process the video file and
		 * keep it for later use.
		 */
		InputFactory (string video_file);

		/**
		 * Image extractor. Return next image-frame from the
		 * input-source to the caller.
		 */
		Mat get_next_frame ();
};

#endif /* __INPUT_FACTORY_HPP__ */
