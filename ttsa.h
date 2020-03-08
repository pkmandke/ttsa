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

namespace nttsa{

class TTSA{
    
    public:

    TTSA(int n = 4, int allocate_memory = 1);
    ~TTSA();
    void allocate_mem();
    
    // Core
    void train(float T);
    bool generateSchedule(std::vector<std::tuple<int, int> > , int *);
    void randomSchedule();
    void init_S_from_file(std::ifstream &); 
    void init_D_from_file(std::ifstream &);
    float get_cost(int *); // Get cost for given schedule
    int nbv(int *); // Number of atmost and no-constraints violations
    bool iter_gensched(std::vector<std::tuple<int, int> > );
    // Neighborhood exploration utlities
    void swapHomes(int *, int, int);
    void swapTeams(int *, int, int );
    void swapRounds(int *, int, int);
    void partialSwapRounds(int*, int, int, int);
    void partialSwapTeams(int *, int, int, int);
    
    // Constraints and feasibility
    bool isAtmost(int *);
    bool isNorepeat(int *);
    bool isFeasible(int *);
    
    // Getter setter and misc utils
    int *get_S();
    int get_n();
    int get_runs();
    void reset_S();
    int *get_dist();
    void reset_dist();

    private:
    
    int *S; // Current Schedule
    int *dist; // Distance matrix
    int n, runs; // n is the # of teams and runs are total rounds = 2 * n - 2
    float start_T, w; // initial temperature
}; // Class TTSA ends

void remove_from_vector(std::vector<std::tuple<int, int> > & , std::tuple<int, int>);
float f_func(int); // The sub-linear function f(v).

// template int remove_from_vector(std::vector<std::tuple<int, int> > &, std::tuple<int, int> &);

void display_S(int *, int, int);
void display_D(int *, int); // Display the distance matrix
void swapInts(int *, int *);
int sign_of(int);
int find_pos(int *, int, int);
}
#endif
