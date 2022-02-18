# Copyright 2011 Brown University Robotics.
# Copyright 2017 Open Source Robotics Foundation, Inc.
# All rights reserved.
#
# Software License Agreement (BSD License 2.0)
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
#  * Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#  * Redistributions in binary form must reproduce the above
#    copyright notice, this list of conditions and the following
#    disclaimer in the documentation and/or other materials provided
#    with the distribution.
#  * Neither the name of the Willow Garage nor the names of its
#    contributors may be used to endorse or promote products derived
#    from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

import sys
from typing import Optional, List, Tuple

import rclpy
from rclpy.node import Node
from lgsvl_msgs.msg import VehicleControlData
from ackermann_msgs.msg import AckermannDrive


class AckermannToLGSVL(Node):
    def __init__(self):
        super().__init__('ackermann_to_lgsvl_control')
        lgsvl_cmd_topic = self.declare_parameter(
            "lgsvl_cmd_topic", "/vehicle_cmd").value
        ackermann_cmd_topic = self.declare_parameter(
            "ackermann_cmd_topic", "/ackermann_cmd").value
        self.sub_ = self.create_subscription(
            AckermannDrive, ackermann_cmd_topic, self.cmd_callback, 1)
        self.pub_ = self.create_publisher(
            VehicleControlData, lgsvl_cmd_topic, 1)
        self.msg = VehicleControlData()

    def cmd_callback(self, data: AckermannDrive):
        global ackermann_cmd_topic
        global pub

        self.msg.acceleration_pct = data.speed
        self.msg.target_wheel_angle = data.steering_angle

        self.pub_.publish(self.msg)


def main(args=None):
    rclpy.init(args=args)
    node = AckermannToLGSVL()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()
