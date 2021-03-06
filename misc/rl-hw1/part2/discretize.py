import numpy as np
from utils.utils import DiscretizeWrapper


class Discretize(DiscretizeWrapper):
    """
    Discretize class: Discretizes a continous gym environment


    -- VARIABLES/FUNCTIONS YOU WILL NEED TO USE --
        * self.state_points (np.ndarray): grid that contains the real values of the discretization

        * self.obs_n (int): number of discrete points

        * self.transitions (np.ndarray): transition matrix of size (S+1, A, S+1). The last state corresponds to the sink
                                         state
        * self.rewards (np.ndarray): reward matrix of size (S+1, A, S+1). The last state corresponds to the sink state

        * self.get_id_from_coordinates(coordinate_vector) returns the id of the coordinate_vector

        * self.get_state_from_id(id_s): get the continuous state associated to that state id

        * self.get_action_from_id(id_a): get the contiouns action associated to that action id

        * env.set_state(s): resets the environment to the continous state s

        * env.step(a): applies the action a to the environment. Returns next_state, reward, done, env_infos. The
                            last element is not used.
    """

    def get_discrete_state_from_cont_state(self, cont_state):
        """
        Get discrete state from continuous state.
            * self.mode (str): specifies if the discretization is to the nearest-neighbour (nn) or n-linear (linear).

        :param cont_state (np.ndarray): of shape env.observation_space.shape
        :return: A tuple of (states, probs). states is a np.ndarray of shape (1,) if mode=='nn'
                and (2 ^ obs_dim,) if mode=='linear'. probs is the probabability of going to such states,
                it has the same size than states.
        """
        """INSERT YOUR CODE HERE"""
        cont_state = np.expand_dims(cont_state, axis=-1)
        diff = self.state_points - cont_state
        if self.mode == 'nn':
            id_s = np.argmin(diff**2)
            states = np.take(self.state_points, id_s)
            ids = self.get_id_from_coordinates(states)
            states = np.array([ids])
            probs = np.ones(1)
            #raise NotImplementedError

        elif self.mode == 'linear':

            index = np.argmin(np.square(diff), axis=1)
            states = np.take(self.state_points, index)
            next_states = np.take(self.state_points, index + 1)
            
            x, y = np.take(diff, index)

            probs = np.array([x * y, (1 - x) * y, x * (1 - y), (1 - x) * (1 - y)])
            probs = np.abs(probs)
            probs = probs / np.sum(probs)
            states = np.array([
                    self.get_id_from_coordinates(states[0]), 
                    self.get_id_from_coordinates(next_states[0]),
                    self.get_id_from_coordinates(states[1]), 
                    self.get_id_from_coordinates(next_states[1])
                    ]
                )

            #raise NotImplementedError
            """Your code ends here"""
        else:
            raise NotImplementedError
        return states, probs

    def add_transition(self, id_s, id_a):
        """
        Populates transition and reward matrix (self.transition and self.reward)
        :param id_s (int): discrete index of the the state
        :param id_a (int): discrete index of the the action

        """
        env = self._wrapped_env
        obs_n = self.obs_n

        """INSERT YOUR CODE HERE"""
        s = self.get_state_from_id(id_s)
        #print(s)
        a = self.get_action_from_id(id_a)
        ns, r, done, _ = env.step(a)
        """INSERT YOUR CODE HERE"""
        self.transitions[id_s, id_a] = self.get_id_from_coordinates(ns)
        self.rewards[id_s, id_a] = r
        #raise NotImplementedError

    def add_done_transitions(self):
        """
        Populates transition and reward matrix for the sink state (self.transition and self.reward). The sink state
        corresponds to the last state (self.obs_n or -1).
        """
        """INSERT YOUR CODE HERE"""
        self.transitions[-1] = -1.
        self.rewards[-1] = -1.
        #raise NotImplementedError



