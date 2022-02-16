from .discretize import Discretize
from envs import DoubleIntegratorEnv, MountainCarEnv
        
def get_env(env=DoubleIntegratorEnv(), state_discretization=21, action_discretization=5, mode='nn'):
        return Discretize(env,
                         state_discretization=state_discretization,
                         action_discretization=action_discretization,
                         mode=mode
                         )
