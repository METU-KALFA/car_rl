FROM kovan:devel
FROM ros:foxy-ros-base
ENV ROS_DISTRO foxy



# Remove apt caches
RUN  rm -rf /var/lib/apt/lists/*

WORKDIR /root

# Create an entrypoint file
COPY ./entrypoint.sh /ros_entrypoint.sh
RUN chmod +x /ros_entrypoint.sh
ENTRYPOINT ["/ros_entrypoint.sh"]
CMD ["bash"]

