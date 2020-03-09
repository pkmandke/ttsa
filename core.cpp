/*
 * The core TTSA algorithm.
 */

#include <iostream>
#include <cstdlib>
#include <cstdbool>
#include <random>

#include "ttsa.h"

using namespace std;

nttsa::TTSA::TTSA(int n, int allocate_memory){
/* 
 * Constructor
 */

    this->n = n;
    this->runs = 2 * n - 2;

    if(allocate_memory)
        this->allocate_mem();

    this->reset_S();
    this->reset_dist();
}

void nttsa::TTSA::train(int maxp, int maxc, float temp, float beta, float delta){
    float bestsoFar = get_cost(this->S);
    bool accept;
    int counter = 0;
    int phase = 0;
    int *S_prime = (int *)malloc((n + 1) * (runs + 1) * sizeof(int));

    while(phase < maxp){
        phase = 0;
        counter = 0;
        while(counter < maxc){
            apply_random_move(S_prime);
            new_cost = get_cost(S_prime);
            if(new_cost < get_cost(S)) accept = true;
            else accept = nttsa::sample_prob(temp, delta); // Sample as per probability
            
            if(accept){
                copy_sched(S_prime, S); // Source, dest
                if(new_cost < bestsoFar){
                    counter = 0;
                    phase = 0;
                    bestsoFar = new_cost;
                }
                else counter++;
            }
        }
        phase++;
        temp = temp * beta;
    }
}

void nttsa::TTSA::apply_random_move(int *Sch){
/* Choose a random move and apply it on the given schedule.
 */

    copy_sched(S, Sch); // Copy of current schedule stored in Sch
    
    // Source: https://en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> dis(1, 5); // 5 moves in total
    

    std::random_device rd_team;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen_team(rd_team()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> team_sampler(1, n); // 5 moves in total
    
    std::random_device rd_rnd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen_rnd(rd_rnd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> rnd_sampler(1, runs); // 5 moves in total
    
    switch(dis(gen)){
        case 1:
            swapHomes(Sch, team_sampler(gen_team), team_sampler(gen_team));
            break;
        case 2:
            break;
    }
}

void nttsa::TTSA::copy_sched(int *src, int *dest){
/* Copy schedule from src to dest leaving src untouched.
 */

    int i, j;

    for(i = 1; i <= n; i++)
    for(j = 1; j <= runs; j++)
        dest[j + i * runs] = src[j + i * runs];
}

void nttsa::TTSA::reset_S(){
    
    int i = 1, j = 1;

    for(i = 1;i <= n; i++)
    for(j = 1;j <= runs; j++)
        this->S[j + i * runs] = 0;
}

void nttsa::TTSA::reset_dist(){

    int i, j;

    for(i = 1; i <= n; i++)
        for(j = 1; j <= n; j++)
            dist[j + i * n] = 0;
}

void nttsa::TTSA::allocate_mem(){
/*
 * Allocate required memory dynamic.
 */
    this->dist = (int *)malloc((n + 1) * (n + 1) * sizeof(int)); // Allocate memory for distance matrix
    this->S = (int *)malloc((n + 1) * (runs + 1) * sizeof(int)); // Allocate 1 row and column extra to allow 1 indexing.
}

nttsa::TTSA::~TTSA(){

    free(this->S);
    free(this->dist);
}
