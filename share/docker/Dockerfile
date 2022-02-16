FROM ros:foxy-ros-base

# install ros build tools
RUN apt-get update && apt-get install -y \
      python3-colcon-common-extensions vim apt-utils

# install Velodyne-LIDAR ROS2/foxy package dependencies
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y \
    ros-foxy-rviz2 \
    ros-foxy-velodyne \
    ros-foxy-pcl-conversions

# install SVL Bridge ROS2/foxy package dependencies
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y \
    libboost-all-dev \  
    libopencv-dev \
    ros-foxy-lgsvl-bridge
    
# install slam-toolbox ROS2/foxy package dependencies
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y \
    ros-foxy-slam-toolbox \
    ros-foxy-xacro \
    ros-foxy-robot-localization \
    ros-foxy-robot-state-publisher-gui
    
# Remove apt caches
RUN  rm -rf /var/lib/apt/lists/*

# clone ros package repo
ENV ROS_WS /root/catkin_ws
RUN mkdir -p $ROS_WS/src
WORKDIR $ROS_WS

# install ros2 demo codes
#RUN git -C src clone \
#      -b master \
#      https://github.com/ros2/demos.git
#
# install ros2 demo package dependencies
#RUN apt-get update && \
#    rosdep update && \
#    rosdep install -y \
#      --from-paths \
#        src/demos/demo_nodes_cpp \
#        src/demos/demo_nodes_py \
#      --ignore-src && \
#    rm -rf /var/lib/apt/lists/*
#

RUN git -C src clone \
        --single-branch -b foxy-devel \
        https://github.com/lgsvl/ros2-lgsvl-bridge.git 
    #source /opt/ros/foxy/setup.bash && \
#RUN colcon build --cmake-args && \
    #source install/setup.bash 
    #lgsvl_bridge 

WORKDIR $ROS_WS
#RUN colcon build --cmake-args '-DCMAKE_BUILD_TYPE="Release"'
# run ros package launch file
#CMD ["colcon", "build", "--cmake-args", "-DCMAKE_BUILD_TYPE=Release"]
