FROM ros:foxy-ros-base
ENV ROS_DISTRO foxy

# install ros build tools
RUN apt-get update && apt-get install -y \
      python3-colcon-common-extensions vim apt-utils

# install Velodyne-LIDAR ROS2/foxy package dependencies
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y \
    ros-${ROS_DISTRO}-rviz2 \
    ros-${ROS_DISTRO}-velodyne \
    ros-${ROS_DISTRO}-pcl-conversions

# install SVL Bridge ROS2/foxy package dependencies
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y \
    libboost-all-dev \  
    libopencv-dev \
    ros-${ROS_DISTRO}-lgsvl-bridge

# clone ros package repo
ENV PRIVATE_WS /root/catkin_ws
RUN mkdir -p ${PRIVATE_WS}/src
WORKDIR ${PRIVATE_WS}

# install ros2 demo codes
RUN git -C ${PRIVATE_WS}/src \
    clone -b master \
    https://github.com/ros2/demos.git

# install ros2 demo package dependencies
RUN pwd
RUN rosdep update && \
    rosdep install -y \
      --from-paths \
        src/demos/demo_nodes_cpp \
        src/demos/demo_nodes_py \
      --ignore-src

RUN git -C ${PRIVATE_WS}/src clone \
        --single-branch -b ${ROS_DISTRO}-devel \
        https://github.com/lgsvl/ros2-lgsvl-bridge.git 

RUN cd ${PRIVATE_WS} && \
    . /opt/ros/${ROS_DISTRO}/setup.sh && \
    colcon build \
      --packages-select ros2-lgsvl-bridge \
      --cmake-args '-DCMAKE_BUILD_TYPE="Release"'

# copy trx6 packages into a new work directory
ENV SHARED_WS /root/host_ws
ENV PACKAGES "${SHARED_WS}/src/packages.txt"
RUN mkdir -p ${SHARED_WS}/src
COPY . ${SHARED_WS}/src
WORKDIR ${SHARED_WS}

RUN echo $(cat ${PACKAGES})
RUN cd ${SHARED_WS}/src && \
    apt update && \
    TERM=linux rosdep install -y \
      --from-paths \
        $(cat ${PACKAGES}) \
      --ignore-src

RUN cd ${SHARED_WS} && \
    . /opt/ros/${ROS_DISTRO}/setup.sh && \
    colcon build \
      --packages-select \
        $(for pkg_path in $(cat ${PACKAGES}); do basename $pkg_path; done)

# Remove source files
RUN rm -r ${SHARED_WS}/src/
RUN mkdir -p ${SHARED_WS}/src/

# Remove apt caches
RUN  rm -rf /var/lib/apt/lists/*

WORKDIR /root

# Create an entrypoint file
COPY entrypoint.sh /root/
RUN chmod +x entrypoint.sh
ENTRYPOINT ["/bin/bash", "/root/entrypoint.sh"]
CMD ["/bin/bash"]

