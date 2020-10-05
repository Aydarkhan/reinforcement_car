## Small project on implementation of reinforcement learning

The project is based on the idea from the class given by LCN at EPFL.
This is a simple environment consisting of a race track (environment)
and a car (agent). The car cannot see and uses reinforcement learning
to learn the track geometry and avoid obstacles.

The car state is represented through it's position (x and y) and velocity
and receives the reward depending on its position within the 2D map.
The car actions include driving in different 2D directions.

The car uses SARSA algorithm to learn the optimal strategy to finish the race as soon as possible.

### Requirements 

- Armadillo
- GTK+ 3.0


### Author
Ruslan Aydakhanov

2013
