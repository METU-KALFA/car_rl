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
from ackermann_msgs.msg import AckermannDrive

if sys.platform == 'win32':
    import msvcrt
else:
    import termios
    import tty


info = """
This node takes keypresses from the keyboard and publishes them
as AckermannDrive messages. It works best with a US keyboard layout.
---------------------------
For moving around:
        w    
    a   s   d

For God Sake, Arrows also work!!!

For gear control:
---------------------------
    f: GEAR_NEUTRAL=0
    r: GEAR_DRIVE=1
    v: GEAR_REVERSE=2
    g: GEAR_PARKING=3
    t: GEAR_LOW=4

For stop press "space"

y/u : increase/decrease max speeds by 10%
h/j : increase/decrease only linear speed by 10%
n/m : increase/decrease only angular speed by 10%

CTRL-C to quit
"""

speedBindings = {
    'w': (1, 0, 0, -1),
    'a': (1, 0, 0, -1),
    's': (0, 0, 0, 1),
    'd': (0, 0, 0, -1),
}

moveBindings = {
    'y': (1.1, 1.1),
    'u': (.9, .9),
    'h': (1.1, 1),
    'j': (.9, 1),
    'n': (1, 1.1),
    'm': (1, .9),
}

throttle = 0.1
steering = 0.1


class TeleopAckerrmannKeyboardNode(Node):
    target_wheel_angle_max = +0.5
    target_wheel_angle_min = -0.5
    acceleration_pct_max = 1.0
    acceleration_pct_min = 0.0

    def __init__(self):
        super().__init__('teleop_ackerrmann_keyboard')
        self.pub = self.create_publisher(AckermannDrive, 'ackermann_cmd', 10)
        self.msg = AckermannDrive()
        self._key = ''
        self.future = None
        print(info)
        self.temperature_timer_ = self.create_timer(
            0.1, self._publish_vehicle_control)

    def add_to_executor(self, executor):
        self.future = executor.create_task(self.__main__)

    def _handle_key(self,
                    key: str,
                    chr_pair: Tuple[str, str],
                    range_pair: Tuple[float, float, float],
                    value: float,
                    step_value: float,
                    release_if_not_pressed: bool = True) -> float:
        chr_min, chr_max = chr_pair
        range_min, range_mid, range_max = range_pair
        epsilon = step_value * 0.5

        if key == chr_max:
            value = min(range_max, value + step_value)
        elif key == chr_min:
            value = max(range_min, value - step_value)
        elif not release_if_not_pressed:
            pass
        elif abs(value - range_mid) > epsilon:
            if value > range_mid:
                value -= epsilon
            else:
                value += epsilon
        else:
            value = 0.0

        return value

    def _publish_vehicle_control(self):
        self.msg.speed = self._handle_key(
            self._key,
            ('s', 'w'),
            (-1.0, 0.0, 1.0),
            self.msg.speed, 
            throttle,
            True)

        self.msg.steering_angle = self._handle_key(
            self._key,
            ('a', 'd'),
            (-1.0, 0.0, +1.0),
            self.msg.steering_angle,
            steering,
            True)

        self._key = ''

        self.get_logger().log(
            f"steering {self.msg.steering_angle}", rclpy.logging.LoggingSeverity.DEBUG)
        self.get_logger().log(
            f"throttle {self.msg.speed}", rclpy.logging.LoggingSeverity.DEBUG)
        self.pub.publish(self.msg)

    def __main__(self):

        settings = self._saveTerminalSettings()

        while True:
            key = self._getKey(settings)
            self.temperature_timer_.cancel()
            self._key = key

            if key in moveBindings.keys():
                pass
            else:
                if (key == '\x03'):
                    break

            self._publish_vehicle_control()
            self.temperature_timer_.reset()

        self._restoreTerminalSettings(settings)
        raise KeyboardInterrupt()

    @staticmethod
    def _getKey(settings):
        if sys.platform == 'win32':
            # getwch() returns a string on Windows
            key = msvcrt.getwch()
        else:
            tty.setraw(sys.stdin.fileno())
            # sys.stdin.read() returns a string on Linux
            key = sys.stdin.read(1)
            termios.tcsetattr(sys.stdin, termios.TCSADRAIN, settings)
        return key

    @staticmethod
    def _saveTerminalSettings() -> Optional[List]:
        if sys.platform != 'win32':
            return termios.tcgetattr(sys.stdin)
        return None

    @staticmethod
    def _restoreTerminalSettings(old_settings) -> None:
        if sys.platform != 'win32':
            termios.tcsetattr(sys.stdin, termios.TCSADRAIN, old_settings)


def main(args=None):
    rclpy.init(args=args)
    node = TeleopAckerrmannKeyboardNode()
    executor = rclpy.executors.MultiThreadedExecutor()
    try:
        node.add_to_executor(executor)
        executor.add_node(node)
        executor.spin()
    except KeyboardInterrupt:
        node.get_logger().log(
            'leaving... ', rclpy.logging.LoggingSeverity.INFO)
    node.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()
