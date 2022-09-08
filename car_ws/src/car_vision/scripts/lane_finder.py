#!/usr/bin/env python3
import cv2
import numpy as np
from numpy.polynomial import polynomial as Poly
 
def make_points(image, line):
    height = image.shape[0]

    slope, intercept = line
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
        print("Left approximated")
        averaged_lines.append(no_line_case_1eft)

    if len(right_fit):
    ##over-simplified if statement (should give you an idea of why the error occurs)
        right_fit_average = np.average(right_fit, axis=0)
        right_line = make_points(image, right_fit_average)
        averaged_lines.append(right_line)
    else:
        print("Right approximated")
        averaged_lines.append(no_line_case_right)
       
    if (not len(left_fit))and(not len(right_fit)):
        print("NO LINE")

    return averaged_lines
 
def canny(img):
    gray = cv2.cvtColor(img, cv2.COLOR_RGB2GRAY)
    kernel = 3
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

image = cv2.imread('/home/tan/Desktop/car_d/car_ws/src/car_vision/car_vision/car_imgs/lane1.png')
print(image.shape)
lane_image = np.copy(image)
lane_canny = canny(lane_image)
cropped_canny = region_of_interest(lane_canny)
lines = cv2.HoughLinesP(cropped_canny, 2, np.pi/180, 100, np.array([]), minLineLength=40,maxLineGap=5)
averaged_lines = average_slope_intercept(image, lines)
if len(averaged_lines) == 2:
    mid_line = [(averaged_lines[0][0]+averaged_lines[1][0])//2,(averaged_lines[0][1]+averaged_lines[1][1])//2,(averaged_lines[0][2]+averaged_lines[1][2])//2,(averaged_lines[0][3]+averaged_lines[1][3])//2]
p = Poly.polyfit([image.shape[0],*mid_line[1::2]],[image.shape[1]//2,*mid_line[::2]],2)
p1 = Poly.polyfit((image.shape[0]-np.array(mid_line[1::2]))*4.0/image.shape[0],(image.shape[1]//2-np.array(mid_line[::2]))*4.0/image.shape[1],(1,2))
x = list(range(image.shape[0]//2,image.shape[0],4))
y = Poly.polyval(x,p)
pts = (np.asarray([y, x]).T).astype(np.int32)
line_image = display_lines(lane_image, averaged_lines+[mid_line])
line_image = cv2.polylines(line_image, [pts], False, (0,255,0))
combo_image = cv2.addWeighted(lane_image, 0.8, line_image, 1, 0)
cv2.imshow("lane_canny", lane_canny)
cv2.imshow("lane_image", lane_image)
cv2.imshow("cropped_canny", cropped_canny)
cv2.imshow("result", combo_image)
print(mid_line)
print(p1)
while True:
    if cv2.waitKey(1) & 0xFF == ord('q'):
            break
cv2.destroyAllWindows()

#
# cap = cv2.VideoCapture("bisikle.mp4")
# while(cap.isOpened()):
#     _, frame = cap.read()
#     canny_image = canny(frame)
#     cropped_canny = region_of_interest(canny_image)
#     lines = cv2.HoughLinesP(cropped_canny, 2, np.pi/180, 100, np.array([]), minLineLength=40,maxLineGap=5)
#     averaged_lines = average_slope_intercept(frame, lines)
#     line_image = display_lines(frame, averaged_lines)
#     combo_image = cv2.addWeighted(frame, 0.8, line_image, 1, 1)
#     cv2.imshow("result", cropped_canny)
#     if cv2.waitKey(1) & 0xFF == ord('q'):
#         break
# cap.release()
# cv2.destroyAllWindows()
