#!/usr/bin/env python3
import rospy
import numpy
from sensor_msgs.msg import PointCloud2
from sensor_msgs.msg import Image
from sensor_msgs.msg import NavSatFix
from sensor_msgs.msg import Imu
from sensor_msgs.msg import LaserScan
from sensor_msgs.msg import Range


class SensorMessages:
    def __init__(self):

        self.frontCameraData = Image()
        rospy.Subscriber('/prius/gps', NavSatFix, self.gpsCB)
        rospy.Subscriber('/prius/imu', Imu, self.imuCB)
        rospy.Subscriber('/prius/center_laser/scan', PointCloud2, self.lidarCB)
        rospy.Subscriber('/prius/front_camera/image_raw',
                         Image, self.frontCameraCB)
        rospy.Subscriber('/prius/right_camera/image_raw',
                         Image, self.rightCameraCB)
        rospy.Subscriber('/prius/back_camera/image_raw',
                         Image, self.backCameraCB)
        rospy.Subscriber('/prius/left_camera/image_raw',
                         Image, self.leftCameraCB)
        rospy.Subscriber('/prius/front_left_laser/scan',
                         LaserScan, self.frontLeftLaserCB)
        rospy.Subscriber('/prius/front_right_laser/scan',
                         LaserScan, self.frontRightLaserCB)
        rospy.Subscriber('/prius/front_sonar/left_far_range',
                         Range, self.sonarFrontLeftFarCB)
        rospy.Subscriber('/prius/front_sonar/left_middle_range',
                         Range, self.sonarFrontLeftMidCB)
        rospy.Subscriber('/prius/front_sonar/right_middle_range',
                         Range, self.sonarFrontRightMidCB)
        rospy.Subscriber('/prius/front_sonar/right_far_range',
                         Range, self.sonarFrontRightFarCB)
        rospy.Subscriber('/prius/back_sonar/left_far_range',
                         Range, self.sonarBackLeftFarCB)
        rospy.Subscriber('/prius/back_sonar/left_middle_range',
                         Range, self.sonarBackLeftMidCB)
        rospy.Subscriber('/prius/back_sonar/right_middle_range',
                         Range, self.sonarBackRightMidCB)
        rospy.Subscriber('/prius/back_sonar/right_far_range',
                         Range, self.sonarBackRightFarCB)

        # rospy.spin()

    def gpsCB(self, data):
        pass

    def imuCB(self, data):
        pass

    def lidarCB(self, data):
        pass

    def frontCameraCB(self, data):
        self.frontCameraData = data

    def rightCameraCB(self, data):
        pass

    def backCameraCB(self, data):
        pass

    def leftCameraCB(self, data):
        pass

    def frontLeftLaserCB(self, data):
        pass

    def frontRightLaserCB(self, data):
        pass

    def sonarFrontLeftFarCB(self, data):
        pass

    def sonarFrontLeftMidCB(self, data):
        pass

    def sonarFrontRightMidCB(self, data):
        pass

    def sonarFrontRightFarCB(self, data):
        pass

    def sonarBackLeftFarCB(self, data):
        pass

    def sonarBackLeftMidCB(self, data):
        pass

    def sonarBackRightMidCB(self, data):
        pass

    def sonarBackRightFarCB(self, data):
        pass
