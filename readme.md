To build the program, build using "make build"
To clean the build, "make clean"

To run:
./run.o <algorithm_code> <location_of_map.txt> <gamma>

<algorithm_code> : 
		1 			-> Policy from Value Iteration
		2 			-> Policy from Policy Iteration
		0 or anything else	-> avg moves for unbiased die

<gamma> : Any value from 0-1
Example usage:

./run.o 1 map1.txt 0.4
[Just copy-paste into terminal]




Working: 
Map can be changed by the user. First line contains no of squares. The following lines contain 2 integers depicting the start and end of the respectives snakes and ladders. Note that snakes and ladders are fundamentally similar.

The die probabilites can be changed in the snakeladder.cpp file within the function snakeladder::InitializeDices().

All the policies are stored into the the log.txt file which can be later used to analyze and observe. 

