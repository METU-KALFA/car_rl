#segmentor
import numpy as np
import cv2
from random import *
from sklearn.linear_model import RANSACRegressor,HuberRegressor,TheilSenRegressor
from sklearn.preprocessing import PolynomialFeatures
from sklearn.pipeline import make_pipeline
from numba import jit
import time

class segmentor:
    def __init__(self, index):
        self.non_zero_left_point_samples = None
        self.non_zero_right_point_samples = None
        self.current_left_point = None
        self.current_right_point = None
        self.lane_center = None
        self.center_of_img = None
        self.index = index

    def update_lane(self,model_left, model_right):
        X=[[self.index]]
        self.current_left_point = model_left.predict(X)[0]#model_left[0]*self.index**2+model_left[1]*self.index+model_left[2]
        self.current_right_point = model_right.predict(X)[0]#model_right[0]*self.index**2+model_right[1]*self.index+model_right[2]
        self.lane_center = (self.current_left_point + self.current_right_point)/2
        
    
    def draw_lane(self,image):
        cv2.line(image,(int(self.current_right_point),int(self.index)),(int(self.current_left_point),int(self.index)),(0,0,50),1)


@jit(nopython=True,cache=True)
def postp(left_pointset_X,left_pointset_Y,right_pointset_X,right_pointset_Y,b_l,b_r):
    x = np.int64(-1)
    
    o = np.int64(60)
    oldpoint = np.int64(-1)
    firstpoint = np.int64(-1)
    for i in range(left_pointset_Y.size):
        if x  == np.int64(-1):
            x = left_pointset_X[i]
            oldpoint = np.int64(-1)
            firstpoint = left_pointset_Y[i]
            b_l[i] = False
            continue
        if not x == left_pointset_X[i]:
            x = left_pointset_X[i]
            oldpoint = np.int64(-1)
            firstpoint = left_pointset_Y[i]
            b_l[i] = False
            continue
        if(((firstpoint < 270 and left_pointset_Y[i] in range(firstpoint-o,firstpoint+o)) or firstpoint >= 270) and((not oldpoint == np.int64(-1) and left_pointset_Y[i] in range(oldpoint-o,oldpoint+o)) or (i+1 < left_pointset_Y.size and left_pointset_X[i+1] == x and left_pointset_Y[i] in range(left_pointset_Y[i+1]-o,left_pointset_Y[i+1]+o)))):
            b_l[i] = True
        else:
            b_l[i] = False
                
    oldpoint = np.int64(-1)
    x = np.int64(-1)
    for i in range(right_pointset_Y.size):
        if  x  == np.int64(-1):
            x = right_pointset_X[i]
            oldpoint = np.int64(-1)
            firstpoint = right_pointset_Y[i]
            b_r[i] = False
            continue
        if not x == right_pointset_X[i]:
            x = right_pointset_X[i]
            oldpoint = np.int64(-1)
            firstpoint = right_pointset_Y[i]
            b_r[i] = False
            continue
        if(((firstpoint > 370 and right_pointset_Y[i] in range(firstpoint-o,firstpoint+o)) or firstpoint <= 370) and ((not oldpoint == np.int64(-1) and right_pointset_Y[i] in range(oldpoint-o,oldpoint+o)) or (i+1 < right_pointset_Y.size and right_pointset_X[i+1] == x and right_pointset_Y[i] in range(right_pointset_Y[i+1]-o,right_pointset_Y[i+1]+o)))):
            b_r[i] = True
        else:
            b_r[i] = False
    return b_l, b_r

@jit(nopython=True,cache=True)
def extract_points(img):
    nz=np.nonzero(img)
    pointset_X = nz[0]
    pointset_Y = nz[1]
    return pointset_X,pointset_Y
@jit(nopython=True,cache=True,parallel=True)
def model_segment(p_y,p_x,a=0,b=0,c=0,is_init=False):
    if is_init:
        b = p_y < 320
    else:
        b = p_y < a*p_x**2+b*p_x+c
    return p_x[b] ,p_x[~b] , p_y[b],p_y[~b]
class LaneDetection:
    def __init__(self):
        segments = []
        for i in range(480):
            s = segmentor(i)
            segments.append(s)
        self.segments = np.array(segments)
        #in the start
        self.model_left = None
        self.model_right = None
        self.model_mid = None
        self.left_pointset = None
        self.right_pointset = None
    @jit(forceobj=True)
    def process(self,img):
        pointset_X,pointset_Y = extract_points(img)
        
        if self.model_mid is None:
            left_pointset_X,right_pointset_X,left_pointset_Y,right_pointset_Y = model_segment(pointset_Y,pointset_X,is_init=True)
        else:
            left_pointset_X,right_pointset_X,left_pointset_Y,right_pointset_Y = model_segment(pointset_Y,pointset_X,self.model_mid[0],self.model_mid[1],self.model_mid[2])
        if(left_pointset_X.any() == 0 or right_pointset_X.any() == 0):
            return None
        #postprocess
        b_l = np.empty(shape=left_pointset_Y.size, dtype=bool)
        b_r = np.empty(shape=right_pointset_Y.size, dtype=bool)
        b_l,b_r = postp(left_pointset_X,left_pointset_Y,right_pointset_X,right_pointset_Y,b_l,b_r)
        left_pointset_X,left_pointset_Y,right_pointset_X,right_pointset_Y = left_pointset_X[b_l],left_pointset_Y[b_l],right_pointset_X[b_r],right_pointset_Y[b_r]
        if(left_pointset_X.any() == 0 or right_pointset_X.any() == 0):
            return None

        #line_right = np.polyfit(right_pointset_X,right_pointset_Y,1)
        #line_left = np.polyfit(left_pointset_X,left_pointset_Y,1)
        r_ran = make_pipeline(PolynomialFeatures(degree=2),RANSACRegressor(max_trials=15,residual_threshold=20,min_samples=3))
        r_ran.fit(right_pointset_X.reshape(-1,1),right_pointset_Y)
        self.model_right=r_ran
        l_ran = make_pipeline(PolynomialFeatures(degree=2),RANSACRegressor(max_trials=15,residual_threshold=20,min_samples=3))
        l_ran.fit(left_pointset_X.reshape(-1,1),left_pointset_Y)
        self.model_left=l_ran
        #np.polyfit(right_pointset_X,right_pointset_Y,2)
        #self.model_left=np.polyfit(left_pointset_X,left_pointset_Y,2)
        #self.model_mid = np.array([(self.model_right[0]+self.model_left[0])/2,(self.model_right[1]+self.model_left[1])/2,(self.model_right[2]+self.model_left[2])/2])
        self.segments[240].update_lane(self.model_left,self.model_right)
#for s in self.segments:
          #  s.update_lane(self.model_left,self.model_right)
        return True

        
    def draw(self,img):
        for s in self.segments:
            s.draw_lane(img)
    def lane_center(self):
        return self.segments[240].lane_center
