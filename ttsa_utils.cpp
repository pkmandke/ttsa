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

#include "ttsa.h"

using namespace std;

int myrandom (int i) { return rand()%i;}

void nttsa::TTSA::randomSchedule(){
/* 
 * RandomSchedule utility used for initializing the Schedule S.
 */
    
    vector<tuple<int, int> > Q; // Stores all positions in the form of (Team, Week) to generate the schedule.

    int i, j;

    for(i = 1; i <= this->n; i++)
    for(j = 1; j <= this->runs; j++)
        Q.push_back(make_tuple(i, j));
    // cout << "Created Q. Now calling generatesched " << Q.size() << endl;
    // exit(0);
    this->generateSchedule(Q, S);
}

bool nttsa::TTSA::generateSchedule(vector<tuple<int, int> > Q, int *S){
/*
 * Generate (recursively) the initial schedule S.
 */
    // cout << "Inside GS " << Q.size() << endl;
    
    if(Q.size() == 0) return true; // If Q is empty return true.
    //cout << "Q size is " << Q.size() << endl;
    int t = get<0>(Q[0]), w = get<1>(Q[0]); // Store the lexicographically minimum <t, w> pair. where t is the team id and w is the week id or run id (both 1-indexed).
    //cout << "t and w are " << t << " " << w << endl;
    int *choices = (int *)malloc((2 * n + 1) * sizeof(int));
    int n_2 = 2 * n;

    int i = 1, c = 1; // Populate choices.
    while(i <= n_2){
        choices[i++] = c;
        choices[i++] = -c;
        c++;
    }
    srand ( unsigned ( std::time(0) ) );
    random_shuffle(&choices[1], &choices[n_2], myrandom); // Shuffle choices
    // cout << "Shuffled choices." << endl;
    for(i = 1; i <= n_2; i++){
        int o = choices[i];
        tuple<int, int> o_w = make_tuple(o, w);
        vector<tuple<int, int> >::iterator finder = find(Q.begin(), Q.end(), o_w);
        // cout << "Finder returned " << o << " " << w << endl;
        if(finder == Q.end()){ // Element <o, w> not in Q
            S[w + t * n] = o; // S[t, w] = o

            if(o > 0) S[w + o * n] = -t;
            else S[w + (-1 * o * n)] = t; // If t is playing with o at o's home, then S[o, w] must be positive.
            // cout << "Q before " << Q.size() << endl;       
            remove_from_vector(Q, make_tuple(t, w)); // Remove this tuple from the vector
            remove_from_vector(Q, make_tuple(o, w));
            remove_from_vector(Q, make_tuple(-o, w));
            // cout << "Q after removal " << Q.size() << endl;
            // exit(0);
            if(generateSchedule(Q, S)){
                free(choices);
                return true;
            } // End generateSch.. if
        } // finder if ends
    } // Main for ends


    free(choices);
    return false;
}

int *nttsa::TTSA::get_S(){

    return this->S;
}

int nttsa::TTSA::get_n(){
    return this->n;
}

int nttsa::TTSA::get_runs(){
    return this->runs;
}
