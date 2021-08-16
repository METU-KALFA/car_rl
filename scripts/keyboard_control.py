#!/usr/bin/env python3

import rospy
from prius_msgs.msg import Control
from sensor_msgs.msg import Joy

class _GetchUnix:
    def __init__(self):
        import tty, sys

    def __call__(self):
        import sys, tty, termios
        fd = sys.stdin.fileno()
        old_settings = termios.tcgetattr(fd)
        try:
            tty.setraw(sys.stdin.fileno())
            ch = sys.stdin.read(1)
        finally:
            termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
        return ch


if __name__ == '__main__':
    rospy.init_node('keyboard_control', anonymous=True)

    pub = rospy.Publisher('/prius', Control, queue_size=100)
    x = _GetchUnix()
    rate = rospy.Rate(1000)
    
    rospy.loginfo('ready for listening to keyboard')
    while not rospy.is_shutdown():
        ch = x().lower()
        if ch:
            msg = Control()
            if ch == 'w':
                msg.throttle = 1.0
            elif ch == 's':
                msg.brake = 1.0
            elif ch == 'a':
                msg.steer = 1.0
            elif ch == 'd':
                msg.steer = -1.0
            elif ch == 'v':
                rospy.loginfo('gears shifted to NEUTRAL')
                msg.shift_gears = Control.NEUTRAL
            elif ch == 'f':
                rospy.loginfo('gears shifted to FORWARD')
                msg.shift_gears = Control.FORWARD
            elif ch == 'r':
                rospy.loginfo('gears shifted to REVERSE')
                msg.shift_gears = Control.REVERSE
            elif ch == ' ':
                msg.shift_gears = Control.NEUTRAL
                msg.brake = 1.0
            elif ch == 'q':
                break
            pub.publish(msg)
        else:
            rate.sleep()
    rospy.loginfo('quiting...')
