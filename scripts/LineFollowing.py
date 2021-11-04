#!/usr/bin/env python
import rospy
import cv2
import sys
from sensor_msgs.msg import Image, CameraInfo
from prius_msgs.msg import Control
from cv_bridge import CvBridge, CvBridgeError
import numpy as np

class cvBridgeDemo():
    def __init__(self):
        self.node_name = "LaneFollowing"
        self.control_msg = Control()
        self.control_msg.header.frame_id = "base_link"

        self.integral = 0
        self.last_value = 0
        self.last_true_value = 0

        # Init ros node
        rospy.init_node(self.node_name)

        # What we do during shutdown
        rospy.on_shutdown(self.cleanup)

        # Create the cv_bridge object
        self.bridge = CvBridge()

        # Subscribe to the camera image and depth topics and set
        # the appropriate callbacks
        self.image_sub = rospy.Subscriber("/prius/front_camera/image_raw", Image, self.image_callback)
        self.pub = rospy.Publisher("/prius", Control)

        rospy.loginfo("Waiting for image topics...")


    def image_callback(self, ros_image):
        # Use cv_bridge() to convert the ROS image to OpenCV format
        try:
            frame = self.bridge.imgmsg_to_cv2(ros_image, "bgr8")
        except CvBridgeError as e:
            print(e)

        # Convert the image to a Numpy array since most cv2 functions require Numpy arrays.
        frame = np.array(frame, dtype=np.uint8)

        # Convert image to HSV color representation and apply hsv filter
        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        hsvfilter = cv2.inRange(hsv, np.array([0, 2, 60]), np.array([180, 255, 255]))

        # Create transformation matrix to birds eye view
        A1 = np.resize(np.array([1, 0, -800/2, 0, 1, -800/2, 0, 0, 0, 0, 0, 1]),(4,3))
        T = np.resize(np.array([1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1000 ,0, 0, 0, 1]),(4,4))
        K = np.resize(np.array([4, 0.0, 800, 0.0, 0, 4, 800, 0.0, 0.0, 0, 1, 0]), (3,4))
        A1 = np.resize(np.array([1, 0, -800/2, 0, 1, -800/2, 0, 0, 0, 0, 0, 1]),(4,3))
        Rx = np.resize(np.array([1, 0, 0, 0, 0, np.cos(0.069777), -np.sin(0.069777), 0, 0, np.sin(0.069777), np.cos(0.069777), 0, 0, 0, 0, 1]),(4,4))
        Ry = np.resize(np.array([np.cos(0), 0, -np.sin(0),0,0,1,0,0,np.sin(0),0,np.cos(0),0,0,0,0,1]),(4,4))
        Rz = np.resize(np.array([np.cos(3.14),-np.sin(3.14),0,0,np.sin(3.14),np.cos(3.14),0,0,0,0,1,0,0,0,0,1]),(4,4))
        R = np.dot(Rx, np.dot(Ry, Rz))
        T = np.resize(np.array([1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, -14 ,0, 0, 0, 1]),(4,4))
        K = np.resize(np.array([4, 0, 800/2, 0, 0, 4, 800/2, 0.0, 0.0, 0, 1, 0]), (3,4))
        M = np.dot(K,np.dot(T,np.dot(R, A1)))

        # Warp image to birds eye view perspective
        birds_eye_view = cv2.warpPerspective(hsvfilter, M, (800,800))[0:400, 0:600]

        # Take parts containing left and right lane and extract coordinates of pixels
        left_lane = birds_eye_view[200:,250:350]
        right_lane = birds_eye_view[200:,450:550]
        left_lane_non_zero = self.delete_outliers(left_lane.nonzero())
        right_lane_non_zero = self.delete_outliers(right_lane.nonzero())
        lefty = left_lane_non_zero[0]
        leftx = left_lane_non_zero[1] + 250
        righty = right_lane_non_zero[0]
        rightx = right_lane_non_zero[1] + 450

        # Fit a second degree poly if there is a point
        if len(leftx) < 3:
            left_fit = []
        else:
            left_fit = np.polyfit(lefty, leftx, 2)

        if len(rightx) < 3:
            right_fit = []
        else:
            right_fit = np.polyfit(righty, rightx, 2)

        # Create and fill the image with fitted lines
        line_img = np.zeros((400,600))
        if (len(right_fit) == 3) and (len(left_fit) == 3):
            for i in range(350, 400):
                if (len(left_fit) != 0) and (len(right_fit) != 0):
                    left_x = int(left_fit[0]*(i-200)**2 + left_fit[1]*(i-200) + left_fit[2])
                    right_x = int(right_fit[0]*(i-200)**2 + right_fit[1]*(i-200) + right_fit[2])
                    if 0 < left_x < 600:
                        line_img[i,left_x] = 1
                    if 0 < right_x < 600:
                        line_img[i,right_x] = 1
                    if (0 < left_x < 600) and (0 < right_x < 600):
                        line_img[i, (left_x+right_x)/2] = 1
        else:
            self.control_msg.steer = 0
            self.control_msg.header.stamp = rospy.Time.now()
            self.pub.publish(self.control_msg)
            print("No lane detected")
            return

        # If bottom of the line image is not empty take the coordinate of the second line (middle line between lanes)
        # and extract from 400 which is the coordinate of the middle point of the car
        if len(line_img[399,:].nonzero()[0]) == 3:
            print(str((line_img[399,:].nonzero()[0][1] - 400)) + " pixels away from center")
            self.away = (line_img[399,:].nonzero()[0][1] - 400)
        else:
            self.control_msg.steer = 0
            self.control_msg.header.stamp = rospy.Time.now()
            self.pub.publish(self.control_msg)
            print("No lane detected")
            return

        # Set P, I and D coefficients for PID controller
        Kp = 0.15
        Ki = 0
        Kd = 0.125

        if abs(self.away - self.last_true_value) > 10:
            self.control_msg.steer = 0
            self.control_msg.header.stamp = rospy.Time.now()
            self.pub.publish(self.control_msg)
            self.last_true_value = self.away
            print("Lane too steep")
            return
        self.last_true_value = self.away
        self.away = self.away * 0.05

        # Calculate integral and derivative
        self.derivative = self.away - self.last_value
        self.integral = self.integral + self.away

        # Calculate the PID controller output and publish the steering angle of the vehicle
        self.control_msg.steer = -(Kp * (self.away) + Kd * self.derivative + Ki * self.integral)
        self.control_msg.header.stamp = rospy.Time.now()
        self.pub.publish(self.control_msg)

        # Store the last value to calculate derivative in next step
        self.last_value = self.away

    def delete_outliers(self, array):
        mean = np.mean(array[1])
        std = np.std(array[1])
        dst = abs(array[1] - mean)
        max_deviation = 3
        not_outliers = dst < max_deviation * std
        return [array[0][not_outliers], array[1][not_outliers]]

    def cleanup(self):
        print("Shutting down")

def main(args):
    cvBridgeDemo()
    rospy.spin()

if __name__ == '__main__':
    main(sys.argv)
