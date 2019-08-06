/**
 * TensorFactory Class Declaration File.
 *
 * This class contains the main work-horse for doing DNN-inference on an image
 * using TensorFlow. It is also responsible for initializing TensorFlow
 * services upon instantiation.
 */
#ifndef __TENSOR_FACTORY_HPP__
#define __TENSOR_FACTORY_HPP__

#include <tensorflow/core/platform/env.h>
#include <tensorflow/core/public/session.h>
#include <tensorflow/core/framework/graph.pb.h>
#include <tensorflow/core/protobuf/meta_graph.pb.h>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;
using namespace tensorflow;

class TensorFactory
{
	Session		*session;

	void check_status (Status status);

	public:
		/**
		 * Default Constructor. Should not be used.
		 */
		TensorFactory ();

		/**
		 * Class Constructor. Initialize TensorFlow graph using the
		 * given DNN-model.
		 */
		TensorFactory (string model_name);

		/**
		 * DNN-inference function. Perform inference on the given
		 * image-frame using TensorFlow library. Return output steering
		 * angle for DeepPicar in radians.
		 */
		float process (Mat frame);
};

#endif /* __TENSOR_FACTORY_HPP__ */
