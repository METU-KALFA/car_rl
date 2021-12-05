# car_rl
Reinforcement autonomous driving project of Kovan Lab.

# AutonomousCar & Gym Environment

In this project we have built an environment which subclasses OpenAI's gym environment. 

* [autonomouscar](./autonomouscar.py) contains the gym environment and interacts with ROS.

* [sensormessages](./sensormessages.py) handles sensor data coming from ROS gazebo simulation.

In order to update any functionality related to autonomouscar, please refer to [autonomouscar](./autonomouscar.py).

Action space consists of 3 inputs.
1. Throttle must be in the range of [0.0, 1.0]
2. Brake must be in the range of [0.0, 1.0]
3. Steer must be in the range of [-1.0, 1.0]
   
In order to estimate the state of the agent you should utilize and combine different data from sensors. You can choose which sensors to use by modifying the autonomouscar's observation space.

***

