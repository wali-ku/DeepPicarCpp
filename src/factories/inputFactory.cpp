#include <inputFactory.hpp>

InputFactory::InputFactory (string video_file)
{
	input_stream.open (video_file);
}

Mat InputFactory::get_next_frame ()
{
	Mat frame;
	input_stream >> frame;

	return frame;
}
