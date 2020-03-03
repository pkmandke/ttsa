/*
 *
 * Header file: ttsa.h
 *
 * Author: Prathamesh Mandke
 *
 * Date created: 02/29/2020
 */

#ifndef TTSA_H_
#define TTSA_H_

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
    
    void train(float T);
    bool generateSchedule(std::vector<std::tuple<int, int> > , int *);
    void randomSchedule();
    
    int *get_S();
    int get_n();
    int get_runs();
    void reset_S();

    private:
    
    int *S;
    int n, runs;
    float start_T;
}; // Class TTSA ends

void remove_from_vector(std::vector<std::tuple<int, int> > & , std::tuple<int, int>);

// template int remove_from_vector(std::vector<std::tuple<int, int> > &, std::tuple<int, int> &);

void display_S(int *, int, int);

}
#endif
