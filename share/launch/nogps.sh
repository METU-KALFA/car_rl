#!/bin/bash                                                                                                                                                                                                 

ros2 run mock_gps mock_gps &
ros2 run goal goal &
ros2 launch realsense2_camera rs_launch.py mode:=manual rgb_camera.profile:=640x480x30  enable_pointcloud:=false enable_accel:=false enable_color:=true enable_depth:=false enable_gyro:=false enable_infra1:=false enable_infra2:=false color_height:=480 color_width:=640 &
ros2 run lane_boundary lane_boundary &
ros2 run velocity_control velocity_control         &
ros2 run subsumption_force subsumption_force       &
ros2 run subsumption_velocity subsumption_velocity &
