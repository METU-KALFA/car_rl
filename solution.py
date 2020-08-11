#!/usr/bin/env python
from autonomouscar import AutonomousCar
import numpy as np
import rospy


print("asd1")
env = AutonomousCar()

print("asd")


def policy_iteration(env, num_episodes):
    for i in range(num_episodes):
        state = env.reset()
        action = [0.5, 0.0, 0.5]
        next_obs, reward, done, _ = env.step(action)
        print(i)


if __name__ == '__main__':
    try:
        while not rospy.is_shutdown():
            policy_iteration(env, 3)
    except rospy.ROSInterruptException:
        pass
