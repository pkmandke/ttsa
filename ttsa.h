/*
 *
 * Header file: ttsa.h
 *
 * Author: Prathamesh Mandke
 *
 * Date created: 02/29/2020
 *
 */

#ifndef TTSA_H_ // Include guard
#define TTSA_H_

#include <iostream>
#include <cstdbool>
#include <vector>
#include <tuple>
#include <bits/stdc++.h>
#include <random>

namespace nttsa{ // Custom namespace

class TTSA{
    
    public:

    TTSA(int n = 4, int allocate_memory = 1); // Constructor
    ~TTSA();
    void allocate_mem(); // Allocate memory for distance and schedules
    
    // Core
    void train(int, int, int, float, float, float, float, float); // The core SA algorithm
    bool generateSchedule(std::vector<std::tuple<int, int> > , int *); // Recursive generate schedule utility
    bool randomSchedule(); // Calls generateSchedule
    void init_S_from_file(std::ifstream &); // Initialize Schedule from a file for debugginf
    void init_D_from_file(std::ifstream &); // Initialize Distance matrix from file
    float get_cost(int *); // Get cost for given schedule
    int nbv(int *); // Number of atmost and no-constraints violations
    
    // Deprecated
    bool iter_gensched(std::vector<std::tuple<int, int> > ); // Iterative implementation of the generateSchedule utility => now deprecated
    
    void apply_random_move(int *); // Apply one of the 5 random moves
    float get_distance(int *); // Get total distance each team needs to travel for a given schedule
    
    // Neighborhood exploration utlities
    void swapHomes(int *, int, int);
    void swapTeams(int *, int, int );
    void swapRounds(int *, int, int);
    void partialSwapRounds(int*, int, int, int);
    bool partialSwapTeams(int *, int, int, int);
    
    // Constraints and feasibility
    bool isAtmost(int *);
    bool isNorepeat(int *);
    bool isFeasible(int *);
    bool verifyRR(int *); // Verify round-robin constraint 

    // Getter setter and misc utils
    int *get_S();
    int get_n();
    int get_runs();
    void reset_S();
    int *get_dist();
    void reset_dist();
    
    private:
    
    void copy_sched(int *, int *);
    int *S; // Current Schedule
    int *dist; // Distance matrix
    int *rr_plus, *rr_minus; // Required for verfying round-robin constraint
    int n, runs; // n is the # of teams and runs are total rounds = 2 * n - 2
    float start_T, w; // initial temperature
}; // Class TTSA ends

void remove_from_vector(std::vector<std::tuple<int, int> > & , std::tuple<int, int>); // Removes given tuple of <t, w> from the vector
float f_func(int); // The sub-linear function f(v).


double what_time_is_it(); // Function for CPU time becnhmarking
bool sample_prob(float, float); // Sample porbability for accepting infeasible solution
void display_S(int *, int, int);
void display_D(int *, int); // Display the distance matrix
void swapInts(int *, int *);
int sign_of(int);
int find_pos(int *, int, int);

}
#endif
