#segmentor
import numpy as np
import cv2
from random import *
from sklearn.linear_model import RANSACRegressor,HuberRegressor
from sklearn.preprocessing import PolynomialFeatures
import time
class QuadraticModel:
    def __init__(self,ransac_model,poly_model):
        self.p = poly_model
        self.r = ransac_model

class Point:
    def __init__(self,x,y):
        self.x = float(x)
        self.y = float(y)

class SegmentVals:
    def __init__(self,ys=None):
        self.y_vals = ys

class SegmentMap:
    x_list = np.empty( 480 , dtype=SegmentVals)
class segmentor:
    def __init__(self, index):
        self.non_zero_left_point_samples = None
        self.non_zero_right_point_samples = None
        self.current_left_point = None
        self.current_right_point = None
        self.lane_center = None
        self.center_of_img = None
        self.index = index
    def update_from_img(self, img):
        index = self.index
        if self.lane_center is not None:
            separator = self.lane_center
        else:
            separator = 320
            self.lane_center = separator
        print(img.shape)
        arr = np.array(np.nonzero(img[index])[0], dtype = np.int32)
        print(arr)
        nonzero_left_points, nonzero_right_points = [arr[arr < separator], arr[~(arr < separator)]]
        #nonzero_left_points, nonzero_right_points = np.unique(nonzero_left_points), np.unique(nonzero_right_points)
        #nonzero_left_points = nonzero_left_points[::-1] #ones closer to middle are moved to the front
        self.non_zero_left_point_samples = nonzero_left_points
        self.non_zero_right_point_samples = nonzero_right_points
       
    def add_to_map(self,s_map_left,s_map_right):
        s_left = SegmentVals(self.non_zero_left_point_samples)
        s_right = SegmentVals(self.non_zero_right_point_samples)
        
        s_map_left.x_list[self.index] = s_left
        s_map_right.x_list[self.index] = s_right
    def update_lane(self,model_left, model_right):
        self.current_left_point = model_left.r.predict( model_left.p.fit_transform(np.array([[self.index]])))[0]
        self.current_right_point = model_right.r.predict( model_right.p.fit_transform(np.array([[self.index]])))[0]
        self.lane_center = (self.current_left_point + self.current_right_point)/2

    
    def draw_lane(self,image):
        cv2.line(image,(int(self.current_right_point),int(self.index)),(int(self.current_left_point),int(self.index)),(0,0,50),1)
    def update_pointset(self,left_X,left_Y,right_X,right_Y):
        for y in self.non_zero_left_point_samples:
            left_X.append(self.index)
            left_Y.append(y)
        for y in self.non_zero_right_point_samples:
            right_X.append(self.index)
            right_Y.append(y)




def better_model(q1, q2):
    if q1.inlier_count > q2.inlier_count:
        return q1
    return q2
def thresholding(q1,point,threshold=35):
    #print(q1.params)
    """ poly = np.array([4*q1.params[0]**2, 6*q1.params[0]*q1.params[1], 4*q1.params[0]*q1.params[2] - 4*q1.params[0]*point.y +2*q1.params[1]**2+2, 2*q1.params[1]*q1.params[2] - 2*q1.params[1]*point.y - 2*point.x])
    x = np.roots(poly)
    y = q1.params[0]*x**2+q1.params[1]*x+q1.params[2]
    d2 = (x-point.x)**2 + (y-point.y)**2
    d = d2 ** 0.5
    real = np.isreal(d)
    real_array = d[real]
    return abs(real_array[0].real) < threshold"""
    return abs(q1.params[0]*point.x**2+q1.params[1]*point.x+q1.params[2]-point.y) < threshold

def get_inliers(pointset,q1,threshold=35):
    inset = []
    for p in pointset:
        if thresholding(q1,p,threshold):
            inset.append(p)
    return np.array(inset)
def get_inlier_count(pointset,q1,threshold=35):
    sum_ = 0
    for p in pointset:
        if thresholding(q1,p,threshold):
            sum_ = sum_ + 1
    return sum_

def fit(rp1,rp2,rp3):
    A = np.matrix([[rp1.x**2, rp1.x, 1],
                   [rp2.x**2, rp2.x, 1],
                   [rp3.x**2, rp3.x, 1]])
    inv_A = A.getI()
    b = np.matrix([[rp1.y],[rp2.y],[rp3.y]])
    ix = inv_A * b
    x = ix.getT()
    x = np.asarray(x).flatten()

    return x
def ransac_iter(segment_map,old_model,pointset):
    y1 = 0
    x1 = 0
    while y1 == 0:
        x1 = randint(320,477)
        if segment_map.x_list[x1].y_vals is not None and segment_map.x_list[x1].y_vals.size > 0:
            y1 = choice(segment_map.x_list[x1].y_vals)
    y2 = 0
    x2 = 0
    while y2 == 0:
        x2 = randint(160,319)
        if segment_map.x_list[x2].y_vals is not None and segment_map.x_list[x2].y_vals.size > 0:
            y2 = choice(segment_map.x_list[x2].y_vals)
    y3 = 0
    x3 = 0
    while y3 == 0:
        x3 = randint(0,159)
        if segment_map.x_list[x3].y_vals is not None and segment_map.x_list[x3].y_vals.size > 0:
            y3 = choice(segment_map.x_list[x3].y_vals)
    p1 = Point(x1,y1)
    p2 = Point(x2,y2)
    p3 = Point(x3,y3)
    
    model = QuadraticModel(fit(p1,p2,p3))
    model.inlier_count = get_inlier_count(pointset,model)
    model = better_model(old_model,model)
    return model
def ransac_final_fit(model,pointset):
    in_points = get_inliers(pointset,model)
    xs = np.array([point.x for point in in_points])
    ys = np.array([point.y for point in in_points])
    return np.polyfit(xs, ys, 2)
class LaneDetection:
    def __init__(self):
        segments = []
        for i in range(480):
            s = segmentor(i)
            segments.append(s)
        self.segments = np.array(segments)
        self.seg_map_left = SegmentMap()
        self.seg_map_right = SegmentMap()
        #in the start
        self.model_left = None
        self.model_right = None
        self.left_pointset = None
        self.right_pointset = None
    def process(self,img):
        start = time.time()
        left_pointset_X  = []
        right_pointset_X = []
        left_pointset_Y = []
        right_pointset_Y = []
        for s in self.segments:
            s.update_from_img(img)
            s.update_pointset(left_pointset_X,left_pointset_Y,right_pointset_X,right_pointset_Y)
        #self.left_pointset = np.array(left_pointset)
        #self.right_pointset = np.array(right_pointset)
        left_pointset_Y = np.asarray(left_pointset_Y)#.reshape(-1, 1)
        left_pointset_X = np.asarray(left_pointset_X).reshape(-1, 1) 
        right_pointset_Y = np.asarray(right_pointset_Y)#.reshape(-1, 1)
        right_pointset_X = np.asarray(right_pointset_X).reshape(-1, 1)
        if(not left_pointset_Y.any() or not right_pointset_Y.any()):
            return None
        
        poly_reg=PolynomialFeatures(degree=2)
        left_X_poly=poly_reg.fit_transform(left_pointset_X)
        poly_reg.fit(left_X_poly,left_pointset_Y)
        left_ran = HuberRegressor()            
        left_ran.fit(left_X_poly,left_pointset_Y)
        self.model_left = QuadraticModel(left_ran,poly_reg)
        poly_reg=PolynomialFeatures(degree=2)
        right_X_poly=poly_reg.fit_transform(right_pointset_X)
        poly_reg.fit(right_X_poly,right_pointset_Y)
        right_ran = HuberRegressor()
        right_ran.fit(right_X_poly,right_pointset_Y)
        self.model_right = QuadraticModel(right_ran,poly_reg)
        
        for s in self.segments:
            s.update_lane(self.model_left,self.model_right)
        end = time.time()
        print("--------")
        print(end-start)
        print("********")
        return True
        """self.model_left = left
        
        self.model_right = QuadraticModel([0,0,0])
        
        self.model_right.params = right_ran.estimator_.coef_[0]
        self.model_right.inlier_count = -1
        """
        
        """
        for i in range(30):
            self.model_right = ransac_iter(self.seg_map_right,self.model_right,self.right_pointset)
        for i in range(30):
            self.model_left  = ransac_iter(self.seg_map_left ,self.model_left ,self.left_pointset )"""
        
    def draw(self,img):
        for s in self.segments:
            s.draw_lane(img)
    def lane_center(self):
        return self.segments[479].lane_center
