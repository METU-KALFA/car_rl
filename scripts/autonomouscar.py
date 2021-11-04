#!/usr/bin/env python3
import gym
import rospy
from gym import spaces
import numpy as np
import sensormessages as sm
from prius_msgs.msg import Control
from std_srvs.srv import Empty




class AutonomousCar(gym.Env):

    def __init__(self):

        rospy.init_node('controller_publisher', anonymous=True)

        self.pub = rospy.Publisher('/prius', Control, queue_size=100)
        self.control_msg = Control()
        self.control_msg.header.seq = 0
        self.observations_space = spaces.Box(
            low=0, high=1, shape=(1, 1), dtype=np.float32)

        self.action_space = spaces.Box(
            low=np.array([0.0, 0.0, -1.0]),
            high=np.array([1.0, 1.0, 1.0]),
            dtype=np.float32)  # throttle, brake, steer

        self.sensorinput = sm.SensorMessages()
        self._reset()

    def reset(self):
        return self._reset()

    def _reset(self):
        try:
            reset_simulation = rospy.ServiceProxy('/gazebo/reset_world', Empty)
            reset_simulation()
            return self._get_obs()
        except rospy.ServiceException as e:
            print(
                "Service call failed: (Your robot couldn't be placed on the initial point:\n) %s" % e)

    def step(self, action):
        return self._step(action)

    def _step(self, action):
        # if the given action is out of boundries error
        assert self.action_space.contains(action)

        self.control_msg.header.seq += 1
        self.control_msg.header.stamp = rospy.Time.now()
        self.control_msg.throttle = action[0]
        self.control_msg.brake = action[1]
        self.control_msg.steer = action[2]
        self.control_msg.shift_gears = 0

        self.pub.publish(self.control_msg)
        # TODO how to determine done & reward
        reward = None
        done = None

        return self._get_obs(), reward, done, {}

    def _get_obs(self):
        arr = self.sensorinput.frontCameraData
        return arr
        # def _reset(self):
