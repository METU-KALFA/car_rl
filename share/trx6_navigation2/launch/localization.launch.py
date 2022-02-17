# Author: Addison Sears-Collins
# Date: August 27, 2021
# Description: Launch a trx6 mobile robot URDF file using Rviz.
# https://automaticaddison.com

import os
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.conditions import IfCondition, UnlessCondition
from launch.substitutions import Command, LaunchConfiguration
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare

def generate_launch_description():
  pkg_share = FindPackageShare(package='trx6_navigation2').find('trx6_navigation2')

  declare_use_sim_time_cmd = DeclareLaunchArgument(
    name='use_sim_time',
    default_value='True',
    description='Use simulation (Gazebo) clock if true')
   

  # Launch Robot Localization 
  robot_localization_cmd = Node(
       package='robot_localization',
       executable='ekf_node',
       name='ekf_filter_node',
       #output='screen',
       parameters=[os.path.join(pkg_share, 'config/ekf.yaml'), {'use_sim_time': LaunchConfiguration('use_sim_time')}]
)

  # Create the launch description and populate
  ld = LaunchDescription()

  # Declare the launch options
  ld.add_action(declare_use_sim_time_cmd)

  # Add any actions
  ld.add_action(robot_localization_cmd)
  return ld

