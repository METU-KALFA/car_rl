# avoid directive
# docker run -t -dp 14550:14550/udp --mount type=bind,source=/home/tan/Desktop/car_d/car_ws/src,target=/car_ws/src tanacar/car_ubuntu:humble
# docker buildx build --platform linux/amd64,linux/arm64 -t tanacar/car_ubuntu --push .
ARG ROS_DISTRO=foxy
FROM ros:${ROS_DISTRO}
ARG ROS_DISTRO
SHELL ["/bin/bash" , "-c"]
RUN sudo apt update && sudo apt-get update
RUN apt-get install ffmpeg libsm6 libxext6 -y
RUN sudo apt install -y nano mc ros-${ROS_DISTRO}-mavros python3 python3-pip
RUN echo "source /opt/ros/${ROS_DISTRO}/setup.bash" > ~/.bashrc
RUN source /opt/ros/${ROS_DISTRO}/setup.bash && ros2 run mavros install_geographiclib_datasets.sh
RUN mkdir -p /car_ws/src
RUN python3 -m pip install --upgrade pip
RUN python3 -m pip install numpy opencv-python
RUN git clone https://github.com/stevengj/nlopt.git && mkdir nlopt/build
WORKDIR /nlopt/build
RUN cmake .. && make && sudo make install
# ros2 run mavros mavros_node --ros-args -p fcu_url:="udp://:14550@"
# ros2 run mavros mavros_node --ros-args -p fcu_url:="/dev/ttyACM0:115200"
CMD [ "/bin/bash" ]
EXPOSE 14550/udp