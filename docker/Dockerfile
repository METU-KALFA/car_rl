FROM ros:foxy-ros-base

# install ros build tools
RUN apt-get update && apt-get install -y \
      python3-colcon-common-extensions \
      vim apt-utils && \
    rm -rf /var/lib/apt/lists/*

# clone ros package repo
ENV ROS_WS /root/catkin_ws
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


# install ros package dependencies
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y \
    ros-foxy-rviz2 \
    ros-foxy-velodyne \
    ros-foxy-pcl-conversions && \
    rm -rf /var/lib/apt/lists/*

# SVL Bridge codes
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y \
    libboost-all-dev \
    ros-foxy-lgsvl-bridge && \
    rm -rf /var/lib/apt/lists/*

RUN git -C src clone \
        --single-branch -b foxy-devel \
        https://github.com/lgsvl/ros2-lgsvl-bridge.git 
    #source /opt/ros/foxy/setup.bash && \
RUN colcon build --cmake-args && \
    source install/setup.bash 
    #lgsvl_bridge 

WORKDIR $ROS_WS
# run ros package launch file
#CMD ["colcon", "build", "--cmake-args", "-DCMAKE_BUILD_TYPE=Release"]
