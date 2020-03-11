/*
 *
 * Header file: ttsa.h
 *
 * Author: Prathamesh Mandke
 *
 * Date created: 02/29/2020
 *
 */

#ifndef TTSA_H_
#define TTSA_H_

#include <iostream>
#include <cstdbool>
#include <vector>
#include <tuple>
#include <bits/stdc++.h>
#include <random>

namespace nttsa{

class TTSA{
    
    public:

    TTSA(int n = 4, int allocate_memory = 1);
    ~TTSA();
    void allocate_mem();
    
    // Core
    void train(int, int, int, float, float, float, float, float);
    bool generateSchedule(std::vector<std::tuple<int, int> > , int *);
    bool randomSchedule();
    void init_S_from_file(std::ifstream &); 
    void init_D_from_file(std::ifstream &);
    float get_cost(int *); // Get cost for given schedule
    int nbv(int *); // Number of atmost and no-constraints violations
    bool iter_gensched(std::vector<std::tuple<int, int> > );
    void apply_random_move(int *);
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
    
    // Random number generators for moves
    // Source: https://en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution
    // std::random_device rd;  //Will be used to obtain a seed for the random number engine
    // std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    //std::uniform_int_distribution<> dis(1, 5); // 5 moves in total

    //std::random_device rd_team;  //Will be used to obtain a seed for the random number engine
    //std::mt19937 gen_team(rd_team()); //Standard mersenne_twister_engine seeded with rd()
    //std::uniform_int_distribution<> team_sampler(1, n); // 5 moves in total
                                
    //std::random_device rd_rnd;  //Will be used to obtain a seed for the random number engine
    //std::mt19937 gen_rnd(rd_rnd()); //Standard mersenne_twister_engine seeded with rd()
    //std::uniform_int_distribution<> rnd_sampler(1, runs); // 5 moves in total
    
    private:
    
    void copy_sched(int *, int *);
    int *S; // Current Schedule
    int *dist; // Distance matrix
    int *rr_plus, *rr_minus; // Required for verfying round-robin constraint
    int n, runs; // n is the # of teams and runs are total rounds = 2 * n - 2
    float start_T, w; // initial temperature
}; // Class TTSA ends

void remove_from_vector(std::vector<std::tuple<int, int> > & , std::tuple<int, int>);
float f_func(int); // The sub-linear function f(v).

// template int remove_from_vector(std::vector<std::tuple<int, int> > &, std::tuple<int, int> &);

bool sample_prob(float, float);
void display_S(int *, int, int);
void display_D(int *, int); // Display the distance matrix
void swapInts(int *, int *);
int sign_of(int);
int find_pos(int *, int, int);
}
#endif
