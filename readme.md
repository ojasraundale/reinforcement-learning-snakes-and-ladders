# About 
This code was made as part of a laboratory assignment for the course Artificial Intelligence at IIT Dharwad in the Spring 2020 Semester. It implements Policy Iteration and Value Iteration Algorithms of RL on a game of Snake and Ladders. The agent has got an option to play 2 unbiased die at each step and wants to determine the optimal dice playing strategy. 

## Build and Run

To build the program, build using `make build`
To clean the build, `make clean`

To run:
``./run.o <algorithm_code> <location_of_map.txt> <gamma>``

<algorithm_code> : 
		1 			-> Policy from Value Iteration
		2 			-> Policy from Policy Iteration
		0 or anything else	-> avg moves for unbiased die

<gamma> : Any value from 0-1

## Example usage:
``./run.o 1 map1.txt 0.4``
[Just copy-paste into terminal]


## Working 
The map can be changed by the user. First line contains no of squares. The following lines contain 2 integers depicting the start and end of the respectives snakes and ladders. Note that snakes and ladders are fundamentally similar.

The die probabilities can be changed in the snakeladder.cpp file within the function snakeladder::InitializeDices().

All the policies are stored into the the log.txt file which can be later used to analyze and observe. 

