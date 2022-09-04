# Car-RL
[![ROS2 Version](https://img.shields.io/badge/ROS2-foxy-red)](https://docs.ros.org/en/foxy/index.html) [![Docker repo](https://img.shields.io/badge/Docker-tanacar/car__ubuntu-blue)](https://hub.docker.com/repository/docker/tanacar/car_ubuntu)

These codes are the result of a 6-week intership at METU-ROMER. These codes are designed for an ackermann vehicle with Nvidia Xavier Kit,Pixhawk4 that contains IMU and GPS sensors, and an Intel Realsense D435 stereo camera.

There are 3 packages located in ./car_ws/src/ called `carrl_interface`, `tan` and `car_vision`.

## The "carrl_interface" package
This package contains the msg, srv and action files that declare custom interface types between the nodes.
### Messages
#### [CurveCoeffs](/car_ws/src/carrl_interface/msg/CurveCoeffs.msg): Contains polynomial curve coefficients.
## The "tan" package
This package contains different control nodes for controlling the car. These nodes are meant to work with [mavros](https://github.com/mavlink/mavros) package.

### Nodes
#### `go_to_pos` node
Draws a 3x3m square by sending position commands via mavros. Intended for testing the offboard control feature.
#### `vel_commander` node
Implements pid control, it requires a point for destination. Needs to be fine-tuned, unstable.
#### `mpc_commander` node
Implements mpc control with custom optimizer, not recommended. It requires a set of points to determine destination path.
#### `mpc_commander_opt` node
Implements mpc control with nlopt optimizer library, it requires a set of points to determine destination path. Not recommended.
#### `mpc_commander_opt_curve` node
Implements mpc control with nlopt optimizer library, it requires a polynomial curve as input. Needs to be fine-tuned, unstable.
#### `tan_commander` node
Implements custom control (didn't know what to call it), it requires a polynomial curve as input. Works great on real-time applications because it depends on input curve updates.
## The "car_vision" package
This package contains code for lane detection.
### Nodes
#### `lane_finder.py` node
Finds lanes with ROI and Canny methods. Works with single images.
#### `lane_finder_video.py` node
Finds lanes with ROI and Canny methods. Works with video input. Outputs a polynoimal curve fitted to follow the lane. Works with `**_curve` control nodes.
#### `road_seg.py` node
An attempt to convert and test image segmentation model from Intel called [road-segmentation-adas-0001](https://docs.openvino.ai/nightly/omz_models_model_road_segmentation_adas_0001.html), tested with OpenVINO environment, works great but cannot be run on Nvidia Xavier. Attemped to convert it to a tenserflow model, unfortunately it did not work.
## Running
### Requirements
#### C++ Dependencies
Eigen library is included in the `tan` package. In case of any error, the library can be installed to the local user via their instuctions.
NLopt library is needed for the optimization features in `tan` package. It can be installed via their instructions.
#### Python Dependencies
Numpy and OpenCV2 packages are required. They can be installed with `python -m pip install numpy opencv-python` command.
#### ROS Package Dependencies
[MAVROS](https://github.com/mavlink/mavros) package is needed to communicate with Pixhawk4. Please follow their install instructions for a complete installation.
### Docker
I provided 2 convenience scripts for running and accessing the docker container.
#### [`dockerrun.sh`](./dockerrun.sh)
This script allows you to run the already built docker image from the [`Dockerfile`](./Dockerfile) in this repo. You should change the bind mount path according to your filesystem as it is configured to work on the car's computer. `--mount type=bind,source={PATH_TO_REPOSITORY}/car_ws/src,target=/car_ws/src`
#### [`dockerbash.sh`](./dockerbash.sh)
This script accesses the running image and opens a bash shell inside the container.
### Codes
In order to communicate with Pixhawk4, mavros has to be run first. Then the control node that uses mavros has to be run. Lastly the vision algorithm node has to be run. I provide 3 convenience scripts to run each node. Written in execution order.
#### [`car_ws/src/mavros.sh`](./car_ws/src/mavros.sh)
This script starts the `mavros_node` thats configured to communicate with Pixhawk4 via a USB cable.
#### [`car_ws/src/commander.sh`](./car_ws/src/commander.sh)
This script starts the `tan_commander_curve` node. It waits for a curve input.
#### [`car_ws/src/cv.sh`](./car_ws/src/cv.sh)
This script starts the `lane_finder_video.py` node that detects lanes and outputs a trajectory path to the control node.
## Results
The system works great when the lane is mostly centered, meaning that the code fails to follow the lane in a sharp turn. `lane_finder_video.py` crashes but I did not have time to debug it :(

Nonetheless here is a test run with the code successfully following a lane for 100 meters. This is a snippet from this [video](./car_ws/src/2022_08_23-10-52_50_result.mp4).
[![](./readme-src/test.gif)]()
## Dreams and Roadmap
If my time with this project was longer, I would want to implement these features:
### Image Segmentation
Image segmantation can classify each pixel of an image and associate it with a class, i.e. road, curb.
This way we would be able to identify road, lanes, vegetation, people, animals and some other objects that we can come across in the field. By only using this method the car would be able to follow lanes and avoid objects it can identify.
### SLAM
SLAM is used for creating a map of the environment you're observing and keep the track of your position while doing so. The IMU sensor data from Pixhawk4 and depth images from the Realsense D435 stereo camera are enough to implement this algorithm.

While GPS and IMU combination gives us an adaquate position estimate, the position estimate from the SLAM can be sent to Pixhawk4 via `/mavros/visual_odom` topic and can be used in sensor fusion for better position estimates.

However, this is not the main reason for implementing SLAM. The map of the environment can be used for the calculation of the optimal path towards some destination point. This way, the car would be able to navigate in a field without roads and guiding features.
### Fusion of SLAM and Image Segmentation
Each pixel identified with image segmantation can be used to generate a 3D occupancy grid map with SLAM. With path planning algorithm such as A* or RRT*, the car would find the optimal path towards a destination.
### Reinforcement Learning
I want to implement a neural network that takes inputs from each one of the aforementioned algorithms and outputs a control action. This way the neural network does not have to learn processing every raw data but instead focus on judging the correct behaviour given the processed inputs. The network would be trained using a simulation and a reward system.

#### Long road ahead!
## Acknowledgement
I want to thank Tunay Baydemir and Moustafa İsmail Hamed Mohamed for working with me and developing this code. Since we didn't used github actively, they don't show up in contributors list but they should, hence the acknowledgement.