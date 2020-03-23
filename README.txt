Travelling Tournament Problem using Simulated Annealing (TTSA)

This program is a C++ implementation of the Travelling Tournament Problem using the Simulated Annealing algorithm.
The implementation closely follows the TTSA algorithm elucidated in the paper by A. Anagnostopoulos et al. available at this HTTP URL: http://aris.me/pubs/ttp.pdf

* Problem Statement

There are n (even) teams that need to be scheduled to play a so-called double round robin (RR) tournament.
A double RR tournament is one in which each team plays with every other team twice once at each team's home.
Thus, given the (pairwise) distance between the homes of all teams, the problem is to come up with a "feasible" schedule for a double RR tournament between the teams.
A schedule of games is said to be feasible iff it satisifies the atmost and no-repeat constraints which are as follows.
1. Atmost constraint: A team can play at the most 3 consecutive home or away games.
2. No-repeat constraint: No two teams can play with each other in consecutive rounds.

* Programmer's guide

The program has been divided into 7 source code files each of which are explained below.
6 of these are cpp files with class definitions and other utility functions while one of them is the header file.
As for the executing the program, main.cpp provides a command-line interface to invoke an instance of the TTSA algorithm that reads the distance matrix of the National League data from the data directory in the current folder and initiates a random schedule to begin execution.
Note that this maye generate quite a bit of output in stdout since a verbosity flag has not been added (yet).
Below is a guide to compile and execute the main program.
                                          
Compile: g++ -std=c++11 -O3 -o my_obj_file main.cpp ttsa_utils.cpp utils.cpp nbd_utils.cpp core.cpp

Execute: ./my_obj_file  num_teams  maxC  maxP  maxR  w_0  T_0  theta  delta  beta

* Author

Prathamesh Mandke
Virginia Tech
https://computing.ece.vt.edu/~pkmandke/
pkmandke AT vt DOT edu
