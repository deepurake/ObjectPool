1) Test cases

test.cpp:
	Contains the basic test case where we create multiple objects using the pool

2) Latency calculations

2.1) latency.cpp:
<Single threaded>
Average latency of object creation : 18477
latency of object using object pool varies. On best scenario it is 
					41760

This can further be improved if we eliminate the mutex to provide thread safety.

2.2) latency_multi_threaded.cpp: 
This case takes arguments num_threads maxCapacity minCapacity
<Multi threaded>
Average Latency of creating an object: 4807ns (10 threads run in parallel)

====================HOW TO COMPILE and RUN ========================

To run any specific test case/latency file, one need to do run the following command 

		“g++ -std=c++14 <file> -o output”

For example 
“g++ -std=c++14 latency_multi_threaded.cpp -o output”
“./output 10 5 10” 
This prints out the latency output of the multi threaded pool based object creation.