/*
 * The core TTSA algorithm.
 */

#include <iostream>
#include <cstdlib>
#include <cstdbool>
#include <random>
#include <limits>

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

void nttsa::TTSA::train(int maxr, int maxp, int maxc, float temp, float beta, float delta, float theta, float init_w){
/*
 * The main SA algorithm.
 */

    float old_cost, new_cost, bestTemp, bestFeasible = std::numeric_limits<float>::infinity(), bestInfeasible = std::numeric_limits<float>::infinity();
    bool accept;
    int nbv_Sp, counter = 0, reheat = 0, phase = 0;
    int nbi = std::numeric_limits<int>::max(), nbf = std::numeric_limits<int>::max();
    int *S_prime = (int *)malloc((n + 1) * (runs + 1) * sizeof(int));
    float bestCost = std::numeric_limits<float>::infinity();

    this->w = init_w;
    double t1 = nttsa::what_time_is_it(); 
    
    // while(reheat <= maxr){
        phase = 0;
        while(phase <= maxp){
            counter = 0;
            while(counter <= maxc){
                apply_random_move(S_prime);
                new_cost = get_cost(S_prime);
                old_cost = get_cost(S);
                nbv_Sp = nbv(S_prime); // Number of violations in the new schedule.
                if((new_cost < old_cost) || (nbv_Sp == 0 && new_cost < bestFeasible) || (nbv_Sp > 0 && new_cost < bestInfeasible)) accept = true;
                else accept = nttsa::sample_prob(temp, abs(new_cost - get_cost(S))); // Sample as per probability
                // cout << "Accept = " << accept << endl;
                if(new_cost < bestCost && isFeasible(S_prime)){
                    // cout << "Current feasible schedule." << endl;
                    // cout << "Counter = " << counter << ", phase = " << phase << ", reheats = " << reheat << endl;
                    // cout << "Current feasible Schedule is: " << endl;
                    // nttsa::display_S(S_prime, n, runs);
                    copy_sched(S_prime, S);
                    cout << "isFeasible method returns => " << isFeasible(S_prime) << endl;
                    cout << "Current cost = " << new_cost << endl;
                    cout << "Number of atmost and no-repeat violations = " << nbv(S_prime) << endl;
                    bestCost = new_cost;
                    cout << "Time since start upto this update: " << nttsa::what_time_is_it() - t1 << "s." << endl; 
                    cout << endl << endl;
                }

                if(accept){
                    copy_sched(S_prime, S); // Source, dest
                    if(nbv_Sp == 0) nbf = std::min(new_cost, bestFeasible);
                    else nbi = std::min(new_cost, bestInfeasible);
                    
                    if((nbf < bestFeasible) || (nbi < bestInfeasible)){
                        reheat = 0;
                        counter = 0;
                        phase = 0;
                        bestTemp = temp;
                        bestFeasible = nbf;
                        bestInfeasible = nbi;
                        if(nbv_Sp == 0) this->w /= theta;
                        else this->w *= delta;
                    }
                    else counter++;
                }
            }        
            // cout << "Phase " << phase << ", bestF = " << bestFeasible << ", bestInf = " << bestInfeasible <<  ", nbi = " << nbi << ", nbf = " << nbf << ", temp = " << temp << endl;
            phase++;
            temp = temp * beta;
        }
        // reheat++;
        // temp = 2 * bestTemp;
        // if(isFeasible(this->S)) return;
    // }

    free(S_prime);
}


void nttsa::TTSA::apply_random_move(int *Sch){
/* Choose a random move and apply it on the given schedule.
 */

    copy_sched(S, Sch); // Copy of current schedule stored in Sch
   

    
   std::random_device rd;  //Will be used to obtain a seed for the random number engine
   std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
   std::uniform_int_distribution<> dis(1, 5); // 5 moves in total
           
   std::random_device rd_team;  //Will be used to obtain a seed for the random number engine
   std::mt19937 gen_team(rd_team()); //Standard mersenne_twister_engine seeded with rd()
   std::uniform_int_distribution<> team_sampler(1, n); // 5 moves in total
                           
   std::random_device rd_rnd;  //Will be used to obtain a seed for the random number engine
   std::mt19937 gen_rnd(rd_rnd()); //Standard mersenne_twister_engine seeded with rd()
   std::uniform_int_distribution<> rnd_sampler(1, runs); // 5 moves in total

   // cout << "Samples are" << endl;
   // cout << dis(gen) << " " << team_sampler(gen_team) << " " << rnd_sampler(gen_rnd) << endl;
   switch(dis(gen)){
        case 1:
            // cout << "Applied 1" << endl;
            swapHomes(Sch, team_sampler(gen_team), team_sampler(gen_team));
            break;
        case 2:
            // cout << "Applied 2" << endl;
            swapTeams(Sch, team_sampler(gen_team), team_sampler(gen_team));
            break;
        case 3:
            // cout << "Applied 3" << endl;
            swapRounds(Sch, rnd_sampler(gen_rnd), rnd_sampler(gen_rnd));
            break;
        case 4:
            // cout << "Applied 4" << endl;
            partialSwapRounds(Sch, team_sampler(gen_team), rnd_sampler(gen_rnd), rnd_sampler(gen_rnd));
            break;
        case 5:
            // cout << "Applied 5" << endl;
            while(!partialSwapTeams(Sch, team_sampler(gen_team), team_sampler(gen_team), rnd_sampler(gen_rnd)));
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
    rr_plus = (int *)malloc((n + 1) * sizeof(int));
    rr_minus = (int *)malloc((n + 1) * sizeof(int));
    this->dist = (int *)malloc((n + 1) * (n + 1) * sizeof(int)); // Allocate memory for distance matrix
    this->S = (int *)malloc((n + 1) * (runs + 1) * sizeof(int)); // Allocate 1 row and column extra to allow 1 indexing.
}

nttsa::TTSA::~TTSA(){

    free(this->S);
    free(this->dist);
    free(this->rr_plus);
    free(this->rr_minus);
}
