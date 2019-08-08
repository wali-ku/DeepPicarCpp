# DeepPicar-C++
DeepPicar is a low-cost autonomous RC car platform using a deep convolutional neural network (CNN). DeepPicar is a small scale replication of NVIDIA's real self-driving car called Dave-2, which drove on public roads using a CNN. DeepPicar uses the same CNN architecture of NVIDIA's Dave-2 and can drive itself in real-time locally on a Raspberry Pi 3.

**This repository** contains the C++ version of the DeepPicar source-code. For the original python version, please refer to this [repository](https://github.com/mbechtel2/DeepPicar-v2).

## Overview of the C++ Version
The C++ version of DeepPicar is primarily designed for real-time performance analysis and experimentation. It can link with the [RTG-Synch](https://github.com/wali-ku/RTG-Synch) library and can be configured to run as a virtual (real-time) gang on a multicore platform.

At the time being, the following features of the original DeepPicar are not implemented in the C++ version:
1. Run in *live mode* i.e., control the actual test-bed in real-time. For this service, interface with RoS is needed
2. Capture input from camera. The current version takes input from a saved video-file only
3. TensorFlow model training. Currently, the executable accepts a pre-built model and performs DNN-inference

Although the above features have not been implemented yet, the C++ version of DeepPicar is designed for extensibility and hence the changes required to add these features (apart from # 3) are quite straight-forward. These features will be added later as needed.

## Pre-requisites
- TensorFlow (built from source)
- OpenCV
- RTG-Synch

## Tested Platforms
The code has been developed and tested on the following platform:
- NVIDIA Jetson TX-2 with L4T (v28.1)

Additionally, the current version of the code has been tested on:
- Intel Core-i5 with Ubuntu 18.1

## Setup
> The code in this repository **cannot be built in a standalone fashion** using the given make script. The folder containing this code **must be** placed under TensorFlow source tree and built from there as per the instructions in this section.

> Please make sure that there is **enough space** on the target platform for hosting and building TensorFlow library from source and OpenCV package before proceeding with the instructions below.

### Phase-1: Building TensorFlow
1. Create a working directory on the target platform's file-system
```
$ mkdir dcar-workspace
$ cd dcar-workspace
```

2. Clone TensorFlow repository
```
$ git clone https://github.com/tensorflow/tensorflow.git
```

3. Follow [these instructions](https://github.com/tensorflow/tensorflow/blob/master/tensorflow/contrib/makefile/README.md) to build TensorFlow from source as per your platform

> The amount of physical memory on your target platform (e.g., Raspberry Pi-3, TX-2) may not be enough to carry-out the compilation of TensorFlow as per instructions [here](https://github.com/tensorflow/tensorflow/blob/master/tensorflow/contrib/makefile/README.md). If your compilation gets terminated due to low memory, consider making more swap-space and using less jobs for running make-script

4. Please make sure to test your TensorFlow build following the directions from the instructions page
---
### Phase-2: Code Relocation and Installing OpenCV
5. Change directory to ```dcar-workspace``` and clone this repository
```
$ git clone https://github.com/wali-ku/DeepPicarCpp.git
```

6. Copy the source-code to TensorFlow
```
$ cp -r DeepPicarCpp tensorflow/tensorflow/contrib/pi_examples/.
```

7. Install OpenCV
```
$ sudo apt install libopencv-dev
```
---
### Phase-3: Building RTG-Synch
8. Clone RTG-Synch repository
```
$ git clone https://github.com/wali-ku/RTG-Synch.git
```

9. Use the make-script to build RTG-Synch library (Requires **sudo**)
```
$ cd RTG-Synch/framework
$ sudo bash
# make
$ <exit-sudo>
$ cd ../..
```
---
### Phase-4: Building DeepPicar Executable
10. Change directory to DeepPicarCpp under Tensorflow
```
cd tensorflow/tensorflow/contrib/pi_examples/DeepPicarCpp
```

11. Ensure that the make-script of DeepPicarCpp contains the correct path of RTG-Synch user-library by checking the variable ```RTGDIR```

12. Change directory to TensorFlow root folder and build DeepPicarCpp
```
cd ../../../..
make -f tensorflow/contrib/pi_examples/DeepPicarCpp/Makefile
```

13. If compilation completes successfully, the executable for DeepPicar will be generated in the ```gen/bin``` directory under DeepPicarCpp folder

## Running DeepPicar
The C++ version of DeepPicar provides the following runtime options:
```
Usage: ./deepPicar <options>

Options:
  -i <input>      Name of the input video file. Must be in
                  the same directory as the executable.
  -m <model>      Name of the tensor-flow model file. Must
                  in the executable directory.
  -l <limit>      Total number of frames to process.
  -f <fps>        Frames per second. This value controls
                  the period of execution for each job.
  -v <id>         Virtual gang ID value. Providing this
                  value signifies that this program is
                  running as part of a virtual gang.
  -r <mbps>       Allowed threshold of memory read ops
                  from co-running best-effort tasks.
  -w <mbps>       Allowed threshold of memory write ops
                  from co-running best-effort tasks.
  -o              Console output switch. If provided then
                  output log is printed to stdout.
  -h              Show this help message

Example:
  $ ./deepPicar -i vid.avi -m mdl.pb -l 100 -f 30
                  Load video from file <vid.avi> and load
                  tensor-flow model from <mdl.pb>. Process
                  100 frames from the video at a rate of
                  1 frame / 33.3 msec (i.e., 30 Frames/sec)
```

A pre-trained TensorFlow model and a test video-file is provided under the input folder of this directory and can be used for quickly testing the DeepPicar executable.

### Documentation
The code is written with inline Doxygen style comments. The provided Doxyfile in this repository can be used to create C++ class documentation in HTML format using the following command:
```
$ sudo apt install doxygen
$ cd DeepPicarCpp
$ doxygen Doxyfile
```
