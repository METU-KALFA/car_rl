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
from .submodules.lane import Lane
delta_t = 0.1
alpha_one = 7
v_max = 0.17
a_max = 1.7
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
    
class cvBridgeDemo(Node):
    def __init__(self):
        super().__init__('LaneFollowing')
        self.node_name = "LaneFollowing"

        self.integral = 0
        self.last_value = 0
        self.last_true_value = 0
        self.prev_car_pos = 0
        
        # Init ros node

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
        self.sub_back = ReentrantCallbackGroup()
        self.serv_group = ReentrantCallbackGroup()
        self.image_sub = self.create_subscription(Image, "/camera/color/image_raw", self.image_callback, 10,callback_group=self.sub_back) 
        self.subsumption_force = self.create_service(ForceBoundaryCheck, "/subsumption_force/goal", self.subsumption_force_callback,callback_group=self.serv_group)
        self.subsumption_vel = self.create_service(VelocityBoundaryCheck, "/subsumption_vel/lane", self.subsumption_vel_callback,callback_group=self.serv_group)
        self.ready_srv = self.create_service(Ready, "/ready/lane", self.ready_callback,callback_group=self.serv_group)
        self.force_srv = self.create_service(Force, '/force/lane', self.force_callback,callback_group=self.serv_group)
        self.image_pub = self.create_publisher(Image,"lane",10)
        self.image_pub2 = self.create_publisher(Image,"ROI",10)
        self.t = 0
        self.controller = PID(1.0,0.0,0.0)
        self.controller.send(None)
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
        dst_now = euclidian_dist([request.x,request.y])
        a = min(dst_now,a_max)
        theta = 0.0
        response.is_overwritten = False
        return response
        if(request.y == 0.0 and request.x==0.0):
            print("LANYAVSAK")
            response.is_overwritten = False
            return response   
        if(request.y == 0.0 or request.x == 0.0):
            theta = 0.0
            print("anan: "+str(theta))
        else:
            theta = request.y/request.x
            print("anan: "+str(theta))
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
        start = timeit.default_timer()
        try:
            frame = self.bridge.imgmsg_to_cv2(ros_image, "bgr8")
        except CvBridgeError as e:
            print(e)
        lane_obj =Lane(orig_frame = frame)    
        
        
        # Perform thresholding to isolate lane lines
        lane_obj.get_line_markings()

        # Plot the region of interest on the image
        #roi = lane_obj.plot_roi(plot=False)
        #self.image_pub2.publish(self.bridge.cv2_to_imgmsg(roi))
        # Perform the perspective transform to generate a bird's eye view
        # If Plot == True, show image with new region of interest
        lane_obj.perspective_transform(plot=False)

        # Generate the image histogram to serve as a starting point
        # for finding lane line pixels
        lane_obj.calculate_histogram(plot=False)  
        
        # Find lane line pixels using the sliding window method 
        
        left_fit, right_fit = lane_obj.get_lane_line_indices_sliding_windows(
          plot=False)
        
        if(type(left_fit) == type(-1)):
            self.t = self.t + 0.03
            print("NO LANE DETECTED")
            return

            # Fill in the lane line
        lane_obj.get_lane_line_previous_window(left_fit, right_fit, plot=False)
        

        # Overlay lines on the original frame
        frame_with_lane_lines = lane_obj.overlay_lane_lines(plot=False)
        c = cv2.cvtColor(frame_with_lane_lines, cv2.COLOR_BGR2RGB)
        self.image_pub.publish(self.bridge.cv2_to_imgmsg(c))
        if(self.im_num != 1800):
            if((self.im_num%30) == 0):
                cv2.imwrite("/root/host_ws/src/lane_ims/"+str(self.im_num/30)+".png",c)
            self.im_num = self.im_num + 1
        # Calculate lane line curvature (left and right lane lines)
        #lane_obj.calculate_curvature(print_to_terminal=False)

        # Calculate center offset                                                                 
        car_pos =lane_obj.calculate_car_position(print_to_terminal=False)
        if(car_pos == None):
            car_pos = -2*self.prev_car_pos
        if(abs(car_pos)> 2):
            car_pos = (abs(car_pos)/car_pos)*2
        print("CAr position"+str(car_pos))
        print(str(timeit.default_timer() - start)+ "s")
        
        displacement_y=self.controller.send([self.t,-car_pos])
        #displacement_y = min(a_max/2,displacement_y)
        self.t = self.t + 0.03
        force1 = np.array([a_max,displacement_y])
        theta = atan2(force1[0],force1[1])
        #if(theta >= 0.27):
         #   force1[0] = 3*np.cos(0.27)
          #  force1[1] = 3*np.sin(0.27)
           # theta = 0.27
       # elif(theta <= -0.27):
        #    force1[0] = 3*np.cos(0.27)
         #   force1[1] = -3*np.sin(0.27)
          #  theta = -0.27
        #if(euclidian_dist(force1) > a_max):
        force1 = normalize(force1)*a_max
        force1[0] = a_max
        force = force1
        self.mutex.acquire()
        self.prev_car_pos = car_pos
        self.ready_counter = self.ready_counter + 1
        self.is_ready = True
        self.force = force
        self.current_target_heading = theta
        self.mutex.release()

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
