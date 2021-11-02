FROM ros:foxy-ros-base

# install ros build tools
RUN apt-get update && apt-get install -y \
      python3-colcon-common-extensions \
      vim apt-utils && \
    rm -rf /var/lib/apt/lists/*

# clone ros package repo
ENV ROS_WS /opt/ros_ws
RUN mkdir -p $ROS_WS/src
WORKDIR $ROS_WS
RUN git -C src clone \
      -b master \
      https://github.com/ros2/demos.git

# install ros package dependencies
RUN apt-get update && \
    rosdep update && \
    rosdep install -y \
      --from-paths \
        src/demos/demo_nodes_cpp \
        src/demos/demo_nodes_py \
      --ignore-src && \
    rm -rf /var/lib/apt/lists/*


WORKDIR /root
# run ros package launch file
#CMD ["ros2", "launch", "demo_nodes_cpp", "talker_listener.launch.py"]
