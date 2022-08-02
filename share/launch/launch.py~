from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='px4_node',
            namespace='px4_node',
            executable='px4_node',
            name='px4_node'
        ),
        Node(
            package='realsense-ros',
            namespace='realsense-ros',
            Executable='realsense-ros',
            name='realsense-ros'
        ),
        Node(
            package='road_boundary',
            namespace='road_boundary',
            executable='road_boundary',
            name='road_boundary'
        ),
        Node(
            package='lane_boundary',
            namespace='lane_boundary',
            executable='lane_boundary',
            name='lane_boundary'
        ),
        Node(
            package='gps_boundary_static',
            namespace='gps_boundary_static',
            executable='gps_boundary_static',
            name='gps_boundary_static'
        ),
        Node(
            package='goal',
            namespace='goal',
            executable='goal',
            name='goal'
        ),
        Node(
            package='subsumption_force',
            namespace='subsumption_force',
            executable='subsumption_force',
            name='subsumption_force'
        ),
        Node(
            package='subsumption_velocity',
            namespace='subsumption_velocity',
            executable='subsumption_velocity',
            name='subsumption_velocity'
        ),
        Node(
            package='velocity_control',
            namespace='velocity_control',
            executable='velocity_control',
            name='velocity_control'
        )
        
    ])
