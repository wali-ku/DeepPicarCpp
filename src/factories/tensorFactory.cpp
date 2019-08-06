#include <tensorFactory.hpp>

TensorFactory::TensorFactory (string model_name)
{
	GraphDef graph;
	Status status;

	status = NewSession (SessionOptions (), &session);
	check_status (status);

	status = ReadBinaryProto (Env::Default (), model_name, &graph);
	check_status (status);

	status = session->Create (graph);
	check_status (status);

	return;
}

void TensorFactory::check_status (Status status)
{
	if (!status.ok ()) {
		cerr << "[ERROR] Operation Failed!" << endl;
		cerr << status.ToString () << endl;
		exit (-1);
	}

	return;
}

float TensorFactory::process (Mat frame)
{
	vector<Tensor> outputs;
	float output_angle_rad;

	TensorShape data_shape ({1, 66, 200, 3});
	Tensor data (DT_FLOAT, data_shape);
	float *p = data.flat<float>().data ();
	Mat temp (66, 200, CV_32FC3, p);

	resize (frame, frame, Size (200, 66));
	frame.convertTo (temp, CV_32F);
	temp = temp / 255.0;

	TF_CHECK_OK (session->Run ({{"input_x", data}}, {"output_y"}, {},
			&outputs));

	auto output_value = outputs[0].tensor<float, 2>();
	output_angle_rad = output_value (0, 0);

	return output_angle_rad;
}
