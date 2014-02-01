Code Written by:
Raihan Hazarika

Files in the directory:
1. variates.h : Header file for the variates.c
2. variates.c : The program to generate the exponential and zipf random variables
3. SDV_sim.c : The SDV simulator program
4. Makefile : To compile the above mentioned files

How to run:
1. Issues a make clean
2. Issues a make
3. The program takes the following 5 parameters as inputs:
	a. Number of Users (Npmr)
	b. Number of channels (Nc)
	c. Number of channel slices (Channel Slices)
	d. Simulation time duration (Simulation Time)[Keep the simulation time as 100000 for the results to converge]
	e. Zipf parameter 

Sample test runs:
1.
	./SDV 257 100 100 100000 0.9

	***** SDV Simulation *****
	Fail request: 0
	Total request: 219455
	Average Number of Streams: 71.749123
	Average Blocking Probability: 0.0000

2.
	./SDV 257 300 300 100000 0.9

	***** SDV Simulation *****
	Fail request: 0
	Total request: 215156
	Average Number of Streams: 116.858289
	Average Blocking Probability: 0.0000

3.
	./SDV 257 150 100 100000 0.9

	***** SDV Simulation *****
	Fail request: 868
	Total request: 217524
	Average Number of Streams: 87.407655
	Average Blocking Probability: 0.0040

4.
 	./SDV 257 200 100 100000 0.9

	***** SDV Simulation *****
	Fail request: 45789
	Total request: 273100
	Average Number of Streams: 92.908226
	Average Blocking Probability: 0.1677




