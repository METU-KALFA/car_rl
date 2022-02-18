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
from typing import Optional, List

import rclpy
from rclpy.node import Node
from lgsvl_self.msgs.self.msg import VehicleControlData

if sys.platform == 'win32':
    import msvcrt
else:
    import termios
    import tty


self.msg = """
This node takes keypresses from the keyboard and publishes them
as VehicleControlData messages. It works best with a US keyboard layout.
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

gearBindings = {
    'f': VehicleControlData.GEAR_NEUTRAL,
    'r': VehicleControlData.GEAR_DRIVE,
    'v': VehicleControlData.GEAR_REVERSE,
    'g': VehicleControlData.GEAR_PARKING,
    't': VehicleControlData.GEAR_LOW,
}

moveBindings = {
    'y': (1.1, 1.1),
    'u': (.9, .9),
    'h': (1.1, 1),
    'j': (.9, 1),
    'n': (1, 1.1),
    'm': (1, .9),
}


class TeleopSteeringKeyboardNode(Node):
    def __init__(self):
        super().__init__('teleop_steering_keyboard')
        self.pub = self.create_publisher(VehicleControlData, 'vehicle_cmd', 10)
        self.msg = VehicleControlData()
        self.__main__()

    def __main__(self):
        settings = self._saveTerminalSettings()

        try:
            print(self.msg)
            while True:
                key = self._getKey(settings)
                if key in speedBindings.keys():
                    self.msg.target_wheel_angle = 0.5
                elif key in gearBindings.keys():
                    self.msg.target_gear = gearBindings[key]
                elif key in moveBindings.keys():
                    pass
                else:
                    if (key == '\x03'):
                        break
                self.pub.publish(self.msg)

        except Exception as e:
            print(e)

        finally:
            stop_msg = VehicleControlData()
            stop_msg
            self.pub.publish(stop_msg)

            self._restoreTerminalSettings(settings)

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
    node = TeleopSteeringKeyboardNode()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()
