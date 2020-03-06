/*
 *
 * Utilities that define operations to explore neighbourhood of the current schedule.
 *
 * These have been included in a file of their own for ease of access since ttsa_utils.cpp can get too cluttered.
 * NOTE: Although these utilities have been defined as methods of the TTSA class, they do not modify the class schedule variable for flexibility of use.
 * In that sense, these are static methods. However, they do have access to the schedule matrix size which is what is used to modify the schedule.
 *
 * Author: Prathamesh
 *
 * Dat Created: 03/04/2020
 */

#include <iostream>
#include <cstdlib>

#include "ttsa.h"

using namespace std;

void nttsa::TTSA::swapHomes(int *Sch, int i, int j){
/*
 *
 * Swap homes of Team i and Team j.
 */

    int run, updates = 0; // Updates can be used for minor optimization by avoiding traversing all rounds/runs when all 4 updates are done.

    for(run = 1; run <= this->runs; run++){
        if(abs(Sch[run + i * this->runs]) == abs(j)) Sch[run + i * runs] = -1 * Sch[run + i * runs];
        if(abs(Sch[run + j * this->runs]) == abs(i)) Sch[run + j * runs] = -1 * Sch[run + j * runs];
    }
}

void nttsa::TTSA::swapRounds(int *Sch, int rk, int rl){
/*
 * Swap ROunds rk and rl in the given schedule S.
 */

    int t = 1, temp;

    for(t = 1; t <= this->n; t++)
        nttsa::swapInts(&Sch[rk + t * runs], &Sch[rl + t * runs]);
    
}

void nttsa::TTSA::swapTeams(int *Sch, int ti, int tj){
/*
 * Swap schedules of team i and team j except when they play each other (home/away).
 */

    int run;
    int s1, s2; // Swap these 2 team's games

    for(run = 1; run <= this->runs; run++){
        
        if(abs(Sch[run + ti * runs]) == abs(tj)) continue; // Skip this round if they are playing each other home/away.

        nttsa::swapInts(&Sch[run + ti * runs], &Sch[run + tj * runs]); // Swap the games


        s1 = Sch[run + ti * runs]; // Ti and Tj after swapping
        s2 = Sch[run + tj * runs];
        
        Sch[run + abs(s1) * runs] = -1 * nttsa::sign_of(s1) * abs(ti);

        Sch[run + abs(s2) * runs] = -1 * nttsa::sign_of(s2) * abs(tj);

    }
}

void nttsa::TTSA::partialSwapRounds(int *Sch, int ti, int rk, int rl){
/*
 * Swap rounds rk and rl for team ti. Update the remaining matches in the same round accordingly.
 */

    int *prev_teams = (int *)malloc(n * sizeof(int)); // Set of teams whose positions/matches have already been swapped until now.
    vector<int> to_swap; // Set of teams to swap yet.

    to_swap.push_back(ti); // Swap ti first

    int cur_swap, i, ptr = 0;
    int new_push, last_pushed = 1;
    
    for(i = 0; i < n; i++) prev_teams[i] = 0; // Record of already swapped teams not to swapped again.

    while(to_swap.size()){ // Until there are teams to swap rounds of
    new_push = 0; // Newly pushed teams to be swapped
    for(i = 0; i < last_pushed; i++){ // Swap the teams pushed in the last round.
        
        cur_swap = to_swap.back(); // Get a team to swap
        to_swap.pop_back();
        prev_teams[ptr++] = cur_swap; // Set it to be swapped

        nttsa::swapInts(&Sch[rk + cur_swap * runs], &Sch[rl + cur_swap * runs]); // Swap schedule of current team
        
        if((nttsa::find_pos(prev_teams, n, abs(Sch[rk + cur_swap * runs])) == -1) && (find(to_swap.begin(), to_swap.end(), abs(Sch[rk + cur_swap * runs])) == to_swap.end())){ // Insert the recently swapped teams to be swapped again only if it hasn't already been swapped or hasn't already beein queued for swapping
            to_swap.insert(to_swap.begin(), abs(Sch[rk + cur_swap * runs]));
            new_push++;
        }
        if(abs(Sch[rk + cur_swap * runs]) != abs(Sch[rl + cur_swap * runs]))
            if((nttsa::find_pos(prev_teams, n, abs(Sch[rl + cur_swap * runs])) == -1) && (find(to_swap.begin(), to_swap.end(), abs(Sch[rl + cur_swap * runs])) == to_swap.end())){
                to_swap.insert(to_swap.begin(), abs(Sch[rl + cur_swap * runs]));
                new_push++;
            }
    } // for ends
    last_pushed = new_push;
    } // while ends

    free(prev_teams);
}

void nttsa::TTSA::partialSwapTeams(int *Sch, int ti, int tj, int r){
/* 
 * Swap team ti with team tj for round r.
 */

    int *swapd_teams = (int *)malloc(n * sizeof(int));
    vector<int> to_swap; // Teams to swap
    int next_rnd = r; // Next round to swap.

    to_swap.push_back(r);
    int ptr = 0, i;

    for(i = 0; i < n; i++) swapd_teams[i] = 0;
    
    while(to_swap.size()){
        
        next_rnd = to_swap.back();
        to_swap.pop_back();
        int find_before_swap = Sch[next_rnd + tj * runs];
        
        swapd_teams[ptr++] = abs(Sch[next_rnd + ti * runs]);
        int nxt = nttsa::find_pos(swapd_teams, n, abs(Sch[next_rnd + tj * runs])); 
        if(nxt == -1){
            for(i = 1; i <= runs; i++)
            if(Sch[i + ti * runs] == Sch[next_rnd + tj * runs])
                to_swap.push_back(i);
        }
        
        nttsa::swapInts(&Sch[next_rnd + ti * runs], &Sch[next_rnd + tj * runs]); // Swap
        nttsa::swapInts(&Sch[next_rnd + abs(Sch[next_rnd + ti * runs]) * runs], &Sch[next_rnd + abs(Sch[next_rnd + tj * runs]) * runs]); // Swap the correspoinding teams as well.
    } // while ends       
        
}
