#!/usr/bin/env python3
import cv2
from datetime import datetime
import numpy as np
from numpy.polynomial import polynomial as Poly
import rclpy
from rclpy.node import Node
from carrl_interface.msg import CurveCoeffs

TEST = False

def make_points(image, line):
    slope, intercept = line
    height = image.shape[0]
    
    y1 = int((height*550)//720)# bottom of the image
    y2 = int(y1*3/5)        # slightly lower than the middle
    x1 = int((y1 - intercept)/slope)
    x2 = int((y2 - intercept)/slope)
    return [x1, y1, x2, y2]
 
def average_slope_intercept(image, lines):
    left_fit    = []
    right_fit   = []
    if lines is None:
        return None
    for line in lines:
        for x1, y1, x2, y2 in line:
            fit = np.polyfit((x1,x2), (y1,y2), 1)
            slope = fit[0]
            intercept = fit[1]
            if slope < 0: # y is reversed in image
                left_fit.append((slope, intercept))
            else:
                right_fit.append((slope, intercept))
    # add more weight to longer lines
    averaged_lines = []
    x = round(image.shape[1]*640/1280)
    no_line_case_1eft = [0, x, round(x* 5 /6), round(x*2/3)]
    no_line_case_right = [round(x*2.1),round(x * 0.95),round(x*1.4),round(x * 0.7)]
    
    if len(left_fit):
        left_fit_average  = np.average(left_fit, axis=0)
        left_line  = make_points(image, left_fit_average)
        averaged_lines.append(left_line)
    else:
        averaged_lines.append(no_line_case_1eft)
    

    if len(right_fit):
    ##over-simplified if statement (should give you an idea of why the error occurs)
        right_fit_average = np.average(right_fit, axis=0)
        right_line = make_points(image, right_fit_average)
        averaged_lines.append(right_line)
    else:
        averaged_lines.append(no_line_case_right)
        
    if (not len(left_fit))and(not len(right_fit)):
        print("NO LINE")

    return averaged_lines
 
 
def canny(img):
    gray = cv2.cvtColor(img, cv2.COLOR_RGB2GRAY)
    kernel = 5
    blur = cv2.GaussianBlur(gray,(kernel, kernel),cv2.BORDER_DEFAULT)
    canny = cv2.Canny(blur, 50, 150)
    return canny
 
def display_lines(img,lines):
    line_image = np.zeros_like(img)
    height = img.shape[0]
    width = img.shape[1]
    
    if lines is not None:
        for x1, y1, x2, y2 in lines:
            cv2.line(line_image,(min(width,max(0,x1)),min(height,max(0,y1))),(min(width,max(0,x2)),min(height,max(0,y2))),(255,0,0),10)
    return line_image
 
def region_of_interest(img):
    height = img.shape[0]
    width = img.shape[1]
    mask = np.zeros_like(img)
  
    triangle = np.array([[
    (0, (height*550)//720),
    ((width*640)//1280, (height*270)//720),
    (width, (height*550)//720),]], np.int32)

    cv2.fillPoly(mask, triangle, 255)
    masked_image = cv2.bitwise_and(img, mask)
    return masked_image

def main(args=None):
    SHOW = False
    RECORD_RAW = True
    RECORD_RESULT = True
    rclpy.init(args=args)
    node = Node("lane_detection")
    pub = node.create_publisher(CurveCoeffs,"/commander/curve",1)
    msg = CurveCoeffs()
    msg.degree = 0 #Start at stopping condition
    
    cap = cv2.VideoCapture("/dev/video4")
    if TEST:
    	cap = cv2.VideoCapture("/car_ws/src/test.mp4")
    	
    	
    	
    now_str = datetime.now().strftime("%Y_%m_%d-%H-%M_%S")
    if RECORD_RAW:
        raw_writer = cv2.VideoWriter(now_str+"_raw.mp4",cv2.VideoWriter_fourcc(*"MP4V"),cap.get(cv2.CAP_PROP_FPS),(int(cap.get(3)),int(cap.get(4))))
    if RECORD_RESULT:
        result_writer = cv2.VideoWriter(now_str+"_result.mp4",cv2.VideoWriter_fourcc(*"MP4V"),cap.get(cv2.CAP_PROP_FPS),(int(cap.get(3)),int(cap.get(4))))
    while(cap.isOpened()):
        _, frame = cap.read()
        if RECORD_RAW:
            raw_writer.write(frame)
        canny_image = canny(frame)
        cropped_canny = region_of_interest(canny_image)
        lines = cv2.HoughLinesP(cropped_canny, 2, np.pi/180, 100, np.array([]), minLineLength=40,maxLineGap=5)
        averaged_lines = average_slope_intercept(frame, lines)
        if averaged_lines:
            mid_line = [(averaged_lines[0][0]+averaged_lines[1][0])//2,(averaged_lines[0][1]+averaged_lines[1][1])//2,(averaged_lines[0][2]+averaged_lines[1][2])//2,(averaged_lines[0][3]+averaged_lines[1][3])//2]
            p = Poly.polyfit([frame.shape[0],*mid_line[1::2]],[frame.shape[1]//2,*mid_line[::2]],2)
            p1 = Poly.polyfit((frame.shape[0]-np.array(mid_line[1::2]))*4.0/frame.shape[0],(frame.shape[1]//2-np.array(mid_line[::2]))*4.0/frame.shape[1],(1,2))
            msg.degree = 2
            msg.coeffs = p1[1:].tolist()
            x = list(range(frame.shape[0]//2,frame.shape[0],4))
            y = Poly.polyval(x,p)
            pts = (np.asarray([y, x]).T).astype(np.int32)
            line_image = cv2.polylines(frame, [pts], False, (0,255,0))
        else:
            msg.degree = 0
        pub.publish(msg)
        line_image = display_lines(frame, averaged_lines)
        combo_image = cv2.addWeighted(frame, 0.8, line_image, 1, 1)
        if SHOW:
            cv2.imshow("result", combo_image)
            if cv2.waitKey(40) & 0xFF == ord('q'):
                break
        if RECORD_RESULT:
            result_writer.write(combo_image)
    cap.release()
    cv2.destroyAllWindows()

if __name__ == '__main__':
    main()
