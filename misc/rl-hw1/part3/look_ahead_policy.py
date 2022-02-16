import numpy as np
from gym import spaces
from part2.look_ahead_policy import LookAheadPolicy as BaseLookAheadPolicy


class LookAheadPolicy(BaseLookAheadPolicy):
    """
    Look ahead policy

    -- UTILS VARIABLES FOR RUNNING THE CODE --
    * look_ahead_type (str): Type of look ahead policy to use

    -- VARIABLES/FUNCTIONS YOU WILL NEED TO USE --
    * self.horizon (int): Horizon for the look ahead policy

    * act_dim (int): Dimension of the state space

    * self.num_elites (int): number of best actions to pick for the cross-entropy method

    * self.value_fun (TabularValueFun):
                - get_values(states): if states is None returns the values of all the states. Otherwise, it returns the
                                      values of the specified states

    * self.get_returns_state(state): It is the same that you implemented in the previous part
    """
    def __init__(self,
                 env,
                 value_fun,
                 horizon,
                 look_ahead_type='tabular',
                 num_acts=20,
                 cem_itrs=10,
                 precent_elites=0.25,
                 ):
        self.env = env
        self.discount = self.env.discount
        self._value_fun = value_fun
        self.horizon = horizon
        self.num_acts = num_acts
        self.cem_itrs = cem_itrs
        self.num_elites = int(num_acts * precent_elites)
        assert self.num_elites > 0
        self.look_ahead_type = look_ahead_type

    def get_action(self, state):
        if self.look_ahead_type == 'tabular':
            action = self.get_action_tabular(state)
        elif self.look_ahead_type == 'rs':
            action = self.get_action_rs(state)
        elif self.look_ahead_type == 'cem':
            action = self.get_action_cem(state)
        else:
            raise NotImplementedError
        return action

    def get_action_cem(self, state):
        """
        Do lookahead in the continous and discrete case with the cross-entropy method..
        :param state (int if discrete np.ndarray if continous)
        :return: best_action (int if discrete np.ndarray if continous)
        """
        mean = 0.0
        std = 1.0
        
        """ INSERT YOUR CODE HERE """
        if isinstance(self.env.action_space, spaces.Discrete):
            lim = lambda x: max(min(x, 2), 0)
            
            weights_pop = [lim(int(mean + std * np.random.randint(0, 3))) for i_weight in range(self.num_acts)]
            rewards = [self.env.step(weights)[1] for weights in weights_pop]

            elite_idxs = np.array(rewards).argsort()[-self.num_elites:]
        
            weights_pop = np.array(weights_pop)
            elite_weights = [weights_pop[idx] for idx in elite_idxs]

            mean = np.array(elite_weights).mean()
            std = np.array(elite_weights).std()
        
            weights_pop = [lim(int(mean + std * np.random.randint(0, 3))) for i_weight in range(self.num_acts)]

            best_action = int(weights_pop[-1])
            #raise NotImplementedError
        else:
            assert self.num_acts is not None
            weights_pop = [mean + std * np.random.randn(*state.shape) for _ in range(self.num_acts)]
            rewards = [self.env.step(weights)[1] for weights in weights_pop]

            elite_idxs = np.array(rewards).argsort()[-self.num_elites:]
        
            weights_pop = np.array(weights_pop)
            elite_weights = [weights_pop[idx] for idx in elite_idxs]

            mean = np.array(elite_weights).mean()
            std = np.array(elite_weights).std()
        
            weights_pop = [mean + std*np.random.randn(*state.shape) for _ in range(self.num_acts)]
            rewards = [self.env.step(weights)[1] for weights in weights_pop]

            elite_idxs = np.array(rewards).argsort()[-self.num_elites:]
        
            weights_pop = np.array(weights_pop)
            elite_weights = [weights_pop[idx] for idx in elite_idxs]


            best_action = weights_pop[-1]

        return best_action

    def get_action_rs(self, state):
        """
        Do lookahead in the continous and discrete case with random shooting..
        :param state (int if discrete np.ndarray if continous)
        :return: best_action (int if discrete np.ndarray if continous)
        """
        """ INSERT YOUR CODE HERE """
        if isinstance(self.env.action_space, spaces.Discrete):
            best_action = np.random.randint(0, 3)
            #raise NotImplementedError
        else:
            assert self.num_acts is not None
            best_action = np.random.randn(*state.shape)
            #raise NotImplementedError

        return best_action
