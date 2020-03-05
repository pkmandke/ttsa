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
        if(abs(Sch[i + run * this->runs]) == abs(j)) Sch[i + run * runs] = -1 * j;
        if(abs(Sch[j + run * this->runs]) == abs(i)) Sch[j + run * runs] = -1 * i;
    }
}

void nttsa::TTSA::swapRounds(int *Sch, int rk, int rl){
/*
 * Swap ROunds rk and rl in the given schedule S.
 */

    int t = 1, temp;

    for(t = 1; t <= this->n; t++)
        nttsa::swapInts(&Sch[t + rk * runs], &Sch[t + rl * runs]);
    
}

void nttsa::TTSA::swapTeams(int *Sch, int ti, int tj){
/*
 * Swap schedules of team i and team j except when they play each other (home/away).
 */

    int run;
    int s1, s2; // Swap these 2 team's games

    for(run = 1; run <= this->runs; run++){
        
        if(abs(Sch[ti + run * runs]) == abs(tj)) continue; // Skip this round if they are playing each other home/away.

        nttsa::swapInts(&Sch[ti + run *runs], &Sch[tj + run * runs]); // Swap the games


        s1 = Sch[ti + run * runs]; // Ti and Tj after swapping
        s2 = Sch[tj + run * runs];
        
        Sch[abs(s1) + run * runs] = -1 * nttsa::sign_of(s1) * abs(ti);

        Sch[abs(s2) + run * runs] = -1 * nttsa::sign_of(s2) * abs(tj);

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
        
        cur_swap = to_swap.pop_back(); // Get a team to swap
        prev_teams[ptr++] = cur_swap; // Set it to be swapped

        nttsa::swapInts(&Sch[cur_swap + rk * runs], &Sch[cur_swap + rl * runs]); // Swap schedule of current team
        
        if((nttsa::fin_pos(prev_teams, n, abs(Sch[cur_swap + rk * runs])) == -1) && (find(to_swap.begin(), to_swap.end(), abs(Sch[cur_swap + rk  * runs])) == to_swap.end())){ // Insert the recently swapped teams to be swapped again only if it hasn't already been swapped or hasn't already beein queued for swapping
            to_swap.insert(to_swap.begin(), abs(Sch[cur_swap + rk * runs]));
            new_push++;
        }
        if(abs(Sch[cur_swap + rk * runs]) != abs(Sch[cur_swap + rl * runs]))
            if((nttsa::fin_pos(prev_teams, n, abs(Sch[cur_swap + rl * runs])) == -1) && (find(to_swap.begin(), to_swap.end(), abs(Sch[cur_swap + rl  * runs])) == to_swap.end())){
                to_swap.insert(to_swap.begin(), abs(Sch[cur_swap + rl * runs]));
                new_push++;
            }
    } // for ends
    last_pushed = new_push;
    } // while ends

    free(prev_teams);
}


