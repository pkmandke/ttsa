/*
 * Utility functions for TTSA.
 *
 * Author: Prathamesh Mandke
 *
 */

#include <iostream>
#include <cstdlib>
#include <vector>
#include <tuple>
#include <cstdbool>
#include <bits/stdc++.h>
#include <algorithm>
#include <ctime>
#include <cmath>
#include <chrono>

#include "ttsa.h"

using namespace std::chrono;
using namespace std;

// Source: https://stackoverflow.com/a/49360677/10182805
 double what_time_is_it()
{
     struct timespec now;
     clock_gettime(CLOCK_REALTIME, &now);
     return now.tv_sec + now.tv_nsec*1e-9;
}

using namespace std;

int myrandom (int i) { return rand()%i;}

bool nttsa::TTSA::randomSchedule(){
/* 
 * RandomSchedule utility used for initializing the Schedule S.
 */
    
    vector<tuple<int, int> > Q; // Stores all positions in the form of (Team, Week) to generate the schedule.

    int i, j, k = 0;
    srand(unsigned(std::time(0)));
    for(i = 1; i <= this->n; i++)
    for(j = 1; j <= this->runs; j++)
        Q.push_back(make_tuple(i, j));
    
    cout << "Calling recursive generate..." << endl;

    if(this->generateSchedule(Q, S)) return true;

    return false;
}


// This method is now deprecated and included for backward compatibility. A recursive version is defined below.
bool nttsa::TTSA::iter_gensched(vector<tuple<int, int> > Q){
/*
 * Please ignore. Deprecated method.
 */

    int t, w, o, j, i, c, k = 0;
    int *choices;
    int n_2;
    while(Q.size()){
        t = get<0>(Q[0]), w = get<1>(Q[0]);
        vector<int> choices;
        n_2 = 2 * n;

        i = 0;
        c = 1; // Populate choices.
        while(c <= n){
            if(c != t){ 
                choices.push_back(c);
                choices.push_back(-c); 
            }
            c++;
        }   
            

    random_shuffle(choices.begin(), choices.end(), myrandom); // Shuffle choices
    int nofree = 0;
    for(i = 0; i < (n_2 - 2); i++){
        o = choices[i];
        bool isPresent = false;
        for(j = 1; j <= n; j++)
        if(abs(o) == abs(S[w + j * runs])){
            isPresent = true;
            break;
        }

        if(isPresent) continue; // team o is already assigned home or away, go to next team for this week
        // if(t >= 7) cout << "Weeknwise vacant " << endl;
        isPresent = false;
        for(j = 1; j <= runs; j++)
        if(o == S[j + abs(t) * runs]){
            isPresent = true;
            break;
        }
         
        if(isPresent) continue;
        nofree = 1;
        
        S[w + abs(t) * runs] = o;
        
        if(o > 0){
            S[w + o * runs] = -t;
        }
        else{
            int o_ = -1 * o;
            S[w + o_ * runs] = t;
        }
        remove_from_vector(Q, make_tuple(t, w)); // Remove this tuple from the vector
        remove_from_vector(Q, make_tuple(abs(o), w));
        break;
    }
    if(nofree == 0){

        return false;
    }
    }
    return true;
}


bool nttsa::TTSA::generateSchedule(vector<tuple<int, int> > Q, int *S){
/*
 * Generate (recursively) the initial schedule S.
 */
    // cout << "Inside GS " << Q.size() << endl;
    
    if(Q.size() == 0) return true; // If Q is empty return true.
    
    // cout << "Q size is " << Q.size() << endl;
    
    int o, j, i, c, k = 0; 
   
    
    int t = get<0>(Q[0]), w = get<1>(Q[0]); // Store the lexicographically minimum <t, w> pair. where t is the team id and w is the week id or run id (both 1-indexed).
    vector<int> choices;
    int n_2 = 2 * n;

    i = 0;
    c = 1; // Populate choices.
    while(c <= n){
    if(c != t){ 
            choices.push_back(c);
            choices.push_back(-c); 
            }
        c++;
    }   
            

    random_shuffle(choices.begin(), choices.end(), myrandom); // Shuffle choices

    for(i = 0; i < (n_2 - 2); i++){
        o = choices[i];
        bool isPresent = false;
        for(j = 1; j <= n; j++)
        if(abs(o) == abs(S[w + j * runs])){
            isPresent = true;
            break;
        }

        if(isPresent) continue; // team o is already assigned home or away, go to next team for this week
        isPresent = false;
        for(j = 1; j <= runs; j++)
        if(o == S[j + abs(t) * runs]){
            isPresent = true;
            break;
        }
         
        if(isPresent) continue;
        
        S[w + abs(t) * runs] = o;
        
        if(o > 0){
            S[w + o * runs] = -t;
        }
        else{
            S[w + abs(o) * runs] = t;
        }
        
        remove_from_vector(Q, make_tuple(t, w)); // Remove this tuple from the vector
        remove_from_vector(Q, make_tuple(abs(o), w));
        
        if(generateSchedule(Q, S)){
            return true;
        }
        
        Q.push_back(make_tuple(t, w));
        Q.push_back(make_tuple(abs(o), w));
        std::sort(Q.begin(), Q.end());
        
        S[w + abs(t) * runs] = 0;
        S[w + abs(o) * runs] = 0;
    }
        
    return false;
}


int nttsa::TTSA::nbv(int *Sch){
/*
 * Computes the number of atmost and no-repeat constraints in given schedule.
 */

    int num_v = 0; // Number of violations

    // Compute atmost constraint violations.

    int tm, rnd, h, w;

    for(tm = 1; tm <= n; tm++){
        h = 0;
        w = 0;
        if(nttsa::sign_of(Sch[1 + tm * runs]) == -1) w = 1; // First game is away
        else h = 1; // First game is home

        for(rnd = 2; rnd <= runs; rnd++){
            if(nttsa::sign_of(Sch[rnd + tm * runs]) == nttsa::sign_of(Sch[rnd - 1 + tm * runs])){ // two consecutive home/away games found
                if(nttsa::sign_of(Sch[rnd + tm * runs]) == -1) w += 1;
                else h += 1;
            }
            else{
                if(nttsa::sign_of(Sch[rnd + tm * runs]) == -1){
                    h = 0; // If two consec games are on not same h/w, then set home counter to zer id current game is away
                    w = 1;
                }
                else{
                    w = 0;
                    h = 1;
                }
            }
            if(h >= 4 || w >= 4) num_v += 1;
        }
    }

    // Compute number of no-repeat violations.

    for(tm = 1; tm <= n; tm++)
    for(rnd = 2; rnd <= runs; rnd++)
        if(abs(Sch[rnd - 1 + tm * runs]) == abs(Sch[rnd + tm * runs])) num_v += 1;
    
    return num_v;
}

float nttsa::TTSA::get_distance(int *Sch){
/*
 * Returns the total distance travelled by all teams.
 */

  int tm, rnd, tot_cost = 0;

  for(tm = 1; tm <= n; tm++){
    if(nttsa::sign_of(Sch[1 + tm * runs]) == -1) // If the first game is away add the distance from home
        tot_cost += dist[tm + abs(Sch[1 + tm * runs]) * n];
    for(rnd = 2; rnd <= runs; rnd++)
        if((nttsa::sign_of(Sch[rnd - 1 + tm * runs]) == -1) && (nttsa::sign_of(Sch[rnd + tm * runs]) == -1)) // If both previous and current games are away
            tot_cost += dist[abs(Sch[rnd - 1 + tm * runs]) + abs(Sch[rnd + tm * runs]) * n];
        else if(nttsa::sign_of(Sch[rnd - 1 + tm * runs]) == -1)
            tot_cost += dist[abs(Sch[rnd - 1 + tm * runs]) + tm * n];
        else if(nttsa::sign_of(Sch[rnd + tm * runs]) == -1)
            tot_cost += dist[abs(Sch[rnd + tm * runs]) + tm * n];
    if(nttsa::sign_of(Sch[runs + tm * runs]) == -1) tot_cost += dist[tm + abs(Sch[runs + tm * runs]) * n]; // If last game for this team is away, then compute distance to home.
  }

  return tot_cost;
}


float nttsa::TTSA::get_cost(int *Sch){
/* 
 * Return the (modified) cost C(S) for the given schedule.
 */

  if(isFeasible(Sch)) return (float)get_distance(Sch);

  return sqrt(pow(get_distance(Sch), 2.0) + pow(this->w * nttsa::f_func(nbv(Sch)), 2.0));
}


bool nttsa::TTSA::verifyRR(int *Sch){
/*
 * Return true iff the given schedule satisfies round-robin constraint.
 */
    int sum, i, j, k, rr_sum;

    for(k = 1; k <= n; k++){
        rr_plus[k] = 0;
        rr_minus[k] = 0;
    }

    for(i = 1; i <= n; i++){
        sum = 0;
        rr_sum = 0;
        for(j = 1; j <= runs; j++)
            if(abs(Sch[j + i * runs]) == i) return false;
            else{
                sum += Sch[j + i * runs];
                if(Sch[j + i * runs] < 0) rr_minus[abs(Sch[j + i * runs])] = 1;
                else if(Sch[j + i * runs] > 0) rr_plus[Sch[j + i * runs]] = 1;
                else return false;
            }
        if(sum != 0) return false;
        if(rr_plus[i] != 0) return false;
        if(rr_minus[i] != 0) return false;

        for(k = 1; k <= n; k++){
            rr_sum += rr_plus[k];
            rr_sum += rr_minus[k];
            rr_plus[k] = 0;
            rr_minus[k] = 0;
        }
        if(rr_sum != (2 * (n - 1))) return false;
    }

    return true;
}


bool nttsa::TTSA::isAtmost(int *Sch){
/* 
 * This method returns true iff the atmost constraint is satisfied.
 */
    int tm, rnd, h, w;

    for(tm = 1; tm <= n; tm++){
        h = 0;
        w = 0;
        if(nttsa::sign_of(Sch[1 + tm * runs]) == -1) w = 1; // First game is away
        else h = 1; // First game is home

        for(rnd = 2; rnd <= runs; rnd++){
            if(nttsa::sign_of(Sch[rnd + tm * runs]) == nttsa::sign_of(Sch[rnd - 1 + tm * runs])){ // two consecutive home/away games found
                if(nttsa::sign_of(Sch[rnd + tm * runs]) == -1) w += 1;
                else h += 1;
            }
            else{
                if(nttsa::sign_of(Sch[rnd + tm * runs]) == -1){
                    h = 0; // If two consec games are on not same h/w, then set home counter to zer id current game is away
                    w = 1;
                }
                else{
                    w = 0;
                    h = 1;
                }
            }
            if(h >= 4 || w >= 4) return false;
        }
    }

    return true;
}

bool nttsa::TTSA::isNorepeat(int *Sch){
/*
 * This method returns true iff the no repeat constraint is satisfied.
 */

    int tm, rnd;

    for(tm = 1; tm <= n; tm++)
    for(rnd = 2; rnd <= runs; rnd++)
        if(abs(Sch[rnd - 1 + tm * runs]) == abs(Sch[rnd + tm * runs])) return false;
    return true;
}

bool nttsa::TTSA::isFeasible(int *Sch){
/* 
 * Return true iff the schedule Sch is feasible, that is, iff both the atmost and norepeat constraints are satisfied.
 */
    if(verifyRR(Sch) && isNorepeat(Sch) && isAtmost(Sch)) return true;

    return false;
}

void nttsa::TTSA::init_S_from_file(ifstream &fin){
/* 
 * Initializing the schedule from a file (instead of random) to help debugging.
 */
    int i, j;
    char ch;

    for(i = 1; i <= n; i++)
    for(j = 1; j <= runs; j++){
        vector<char> num;
        while(fin.get(ch)){
            if(ch == ' ' || ch == '\n') break;
            num.push_back(ch);
        }
        if(num.size())
            S[j + i * runs] = std::stoi(std::string(num.begin(), num.end()));
    }
}

void nttsa::TTSA::init_D_from_file(ifstream &fin){
/*
 * Initialize the n by n distance matrix from a file.
 */

    int i, j;
    char ch;
    bool got_dist;

    for(i = 1; i <= n; i++)
    for(j = 1; j <= n; j++){
        vector<char> num;
        got_dist = false;
        while(fin.get(ch)){
            if(ch == ' ' || ch == '\n'){
                if(got_dist) break;
                else continue;
            }
            num.push_back(ch);
            got_dist = true;
        }
        if(num.size()){
            dist[j + i * n] = std::stoi(std::string(num.begin(), num.end())); // Read number and convert to int
        }
    }
}

int *nttsa::TTSA::get_S(){
    return this->S;
}

int *nttsa::TTSA::get_dist(){
    return this->dist;
}

int nttsa::TTSA::get_n(){
    return this->n;
}

int nttsa::TTSA::get_runs(){
    return this->runs;
}
