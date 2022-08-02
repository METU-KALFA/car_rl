#!/usr/bin/env python
#import rospy
from cmath import inf
from copy import copy
from math import atan2
import rclpy
from rclpy.node import Node
from rclpy.callback_groups import MutuallyExclusiveCallbackGroup, ReentrantCallbackGroup
from rclpy.executors import MultiThreadedExecutor
import cv2
import sys
from service_interface.srv import Ready, Location, Force, PolyLineIntersectionCheck, ForceBoundaryCheck, VelocityBoundaryCheck
from geometry_msgs.msg import Polygon, Point
from sensor_msgs.msg import Image, CameraInfo
from cv_bridge import CvBridge, CvBridgeError
import numpy as np
import threading
import time
import timeit
from .submodules.lane import *
delta_t = 0.1
alpha_one = 7
v_max = 0.1
a_max = 2.5
delta_x_max = v_max * delta_t
def euclidian_dist(p1):
    return np.sqrt((p1[0])**2 + (p1[1])**2)
def normalize(v):
    norm = np.linalg.norm(v)
    if norm == 0:
        return v
    return v / norm
def PID(Kp, Ki, Kd, MV_bar=0):
    # initialize stored data
    e_prev = 0
    t_prev = -0.1
    I = 0
    
    # initial control
    MV = MV_bar
    
    while True:
        # yield MV, wait for new t, PV, SP
        t, e = yield MV
        
        # PID calculations
        
        P = Kp*e
        I = I + Ki*e*(t - t_prev)
        D = Kd*(e - e_prev)/(t - t_prev)
        
        MV = MV_bar + P + I + D
        
        # update stored data for next iteration
        e_prev = e
        t_prev = t
def perspective_warp(dst_size=(640,480),
		     src=np.float32([(60+165,266),(580-165,266),(0, 375), (640,375)]),
		     dst=np.float32([(1/3,0), (2/3, 0), (1/3,1), (2/3,1)])):
	dst = dst * np.float32(dst_size)
	M = cv2.getPerspectiveTransform(src, dst)
	return M
def i_perspective_warp(dst_size=(640,480),
		     src=np.float32([(60+165,266),(580-165,266),(0, 375), (640,375)]),
		     dst=np.float32([(1/3,0), (2/3, 0), (1/3,1), (2/3,1)])):
	dst = dst * np.float32(dst_size)
	M = cv2.getPerspectiveTransform(dst, src)
	return M
class cvBridgeDemo(Node):
    def __init__(self):
        super().__init__('LaneFollowing')
        self.node_name = "LaneFollowing"

        self.integral = 0
        self.last_value = 0
        self.last_true_value = 0
        self.prev_car_pos = 0
        
        # I9nit ros node

        # What we do during shutdown

        # Create the cv_bridge object
        self.bridge = CvBridge()
        self.is_ready = False
        self.ready_counter = 0
        self.force = (0,0)
        self.mutex = threading.Lock()
        self.poly_right = [0.0001,250.0]
        self.poly_left = [0.0001,450.0]
        self.current_target_heading = 0
        self.im_num = 0
        # Subscribe to the camera image and depth topics and set
        # the appropriate callbacks
        self.sub_back = MutuallyExclusiveCallbackGroup()
        self.serv_group = ReentrantCallbackGroup()
        self.image_sub = self.create_subscription(Image, "/camera/color/image_raw", self.image_callback, 10,callback_group=self.sub_back) 
        self.subsumption_force = self.create_service(ForceBoundaryCheck, "/subsumption_force/goal", self.subsumption_force_callback,callback_group=self.serv_group)
        self.subsumption_vel = self.create_service(VelocityBoundaryCheck, "/subsumption_vel/lane", self.subsumption_vel_callback,callback_group=self.serv_group)
        self.ready_srv = self.create_service(Ready, "/ready/lane", self.ready_callback,callback_group=self.serv_group)
        self.force_srv = self.create_service(Force, '/force/lane', self.force_callback,callback_group=self.serv_group)
        self.image_pub = self.create_publisher(Image,"lane",10)
        self.image_pub2 = self.create_publisher(Image,"ROI",10)
        self.t = 0
        self.controller = PID(0.001,0.0,0.0)
        self.controller.send(None)
        self.M = perspective_warp()
        self.inv_M = i_perspective_warp()
        self.gray_minus = np.full((480, 640, 1), 50, np.uint8)
        self.lane_detector = LaneDetection()
    def ready_callback(self, request, response):
        response.ready = self.is_ready
        self.ready_counter = self.ready_counter - 1
        return response
    def force_callback(self, request, response):
        self.mutex.acquire()
        response.x = float(self.force[0])
        response.y = float(self.force[1])
        if self.ready_counter == 0:
            self.is_ready = False
        self.mutex.release()
        return response
    def subsumption_force_callback(self, request, response):
        # This is the subsumption logic for the force boundary however it will probably change in the future
        # As the clipping is done here. This is an old solution and is not really scalable.
        dst_now = euclidian_dist([request.x,request.y])
        a = min(dst_now,a_max)
        theta = 0.0
        response.is_overwritten = False
        return response
        if(request.y == 0.0 and request.x==0.0):
            response.is_overwritten = False
            return response   
        if(request.y == 0.0 or request.x == 0.0):
            theta = 0.0
        else:
            theta = request.y/request.x
        if theta >= 0 and self.current_target_heading >= 0:
            if theta < self.current_target_heading + 0.09 and theta > self.current_target_heading - 0.09:
                f = normalize([request.x,request.y])*a
                response.x = f[0]
                response.y = f[1]
                response.is_overwritten = True
            else:
                response.is_overwritten = False
        elif theta <= 0 and self.current_target_heading <= 0:
            if -theta < -self.current_target_heading + 0.09 and -theta > -self.current_target_heading - 0.09:
                f = normalize([request.x,request.y])*a
                response.x = f[0]
                response.y = f[1]
                response.is_overwritten = True
            else:
                response.is_overwritten = False
        else:
            response.is_overwritten = False
        return response
    def subsumption_vel_callback(self, request, response):
        response.x = request.x
        response.y = request.y
        return response

    def image_callback(self, ros_image):
        # Use cv_bridge() to convert the ROS image to OpenCV format
        start = time.time()
        try:
            frame = self.bridge.imgmsg_to_cv2(ros_image, "bgr8")
        except CvBridgeError as e:
            print(e)
        #orig = frame.copy()
        # for lane publishing get a dark image
        #img = np.zeros((480,640,3), dtype=np.uint8)
        
        # DARKENED GRAYSCALE
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        gray = cv2.subtract(gray,self.gray_minus)
        # HLS COLOR SPACE
        frame = cv2.cvtColor(frame, cv2.COLOR_BGR2HLS)
        # White and Yellow Masking
        w_m = cv2.inRange(frame,  np.array([0, 170, 0], dtype=np.uint8), np.array([200, 255, 255], dtype=np.uint8))
        y_m = cv2.inRange(frame, np.array([10, 0, 100], dtype=np.uint8), np.array([40, 255, 255], dtype=np.uint8))
        m = cv2.bitwise_or(y_m,w_m)
        # Mask applied over the grayscale image
        gray = cv2.bitwise_and(gray, gray, mask=m)
        # Gaussian Blur
        gray = cv2.blur(gray, (3,3),0)
        # Canny Edge detection
        frame = cv2.Canny(gray, 70, 140)
        # Warp perspective
        frame = cv2.warpPerspective(frame, self.M, (640,480))
	
	# Ready for getting lane lines
        if(self.lane_detector.process(frame) is None):
            print("Not detected")
            return
        center_of_lane = int(self.lane_detector.lane_center())

        # Lane publish 
        #self.lane_detector.draw(img)
        #img = cv2.warpPerspective(img, self.inv_M, (640,480))
        #img = orig + img
        #self.image_pub.publish(self.bridge.cv2_to_imgmsg(img))
        #print("pub")
       # if(self.im_num != 1800):
       #     if((self.im_num%30) == 0):
#                cv2.imwrite("/root/host_ws/src/lane_ims/"+str(self.im_num/30)+".png",img)
 #           self.im_num = self.im_num + 1
        
        displacement_y=self.controller.send([self.t,center_of_lane - 320.0])
        #displacement_y = min(a_max/2,displacement_y)
        self.t = self.t + 0.03
        force1 = np.array([a_max,displacement_y])
        theta = atan2(force1[0],force1[1])
        force1 = normalize(force1)*a_max
        force1[0] = a_max
        force = force1
        self.mutex.acquire()
        self.prev_car_pos = center_of_lane
        self.ready_counter = self.ready_counter + 1
        self.is_ready = True
        self.force = force
        self.current_target_heading = theta
        self.mutex.release()
        end = time.time()
        print((end-start))

    def delete_outliers(self, array):
        mean = np.mean(array[1])
        std = np.std(array[1])
        dst = abs(array[1] - mean)
        max_deviation = 3
        not_outliers = dst < max_deviation * std
        return [array[0][not_outliers], array[1][not_outliers]]

    def cleanup(self):
        print("Shutting down")

def main(args=None):
    rclpy.init(args=args)

    cvNode = cvBridgeDemo()
    executor = MultiThreadedExecutor(num_threads=2)
    executor.add_node(cvNode)
    try:
        executor.spin()
    finally:
        executor.shutdown()

    # Destroy the node explicitly
    # (optional - otherwise it will be done automatically
    # when the garbage collector destroys the node object)
    cvNode.destroy_node()
    cvNode.cleanup()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
