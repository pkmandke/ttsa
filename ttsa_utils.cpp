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

bool tuple_compare (std::tuple <int,int> t1, std::tuple <int,int> t2){
  return (t1<=t2);
}

void nttsa::TTSA::randomSchedule(){
/* 
 * RandomSchedule utility used for initializing the Schedule S.
 */
    
    vector<tuple<int, int> > Q; // Stores all positions in the form of (Team, Week) to generate the schedule.

    int i, j, k = 0;

    for(i = 1; i <= this->n; i++)
    for(j = 1; j <= this->runs; j++)
        Q.push_back(make_tuple(i, j));
    std::sort(Q.begin(), Q.end(), tuple_compare);
    for(i = 1; i <= this->n; i++)
    for(j = 1; j <= this->runs; j++){
        cout << get<0>(Q[k]) << " " << get<1>(Q[k]) << endl; // cout << "Created Q. Now calling generatesched " << Q.size() << endl;
        k++;
    }
    srand ( unsigned ( std::time(0) ) );
    // exit(0);
    this->generateSchedule(Q, S);
}

bool nttsa::TTSA::generateSchedule(vector<tuple<int, int> > Q, int *S){
/*
 * Generate (recursively) the initial schedule S.
 */
    // cout << "Inside GS " << Q.size() << endl;
    
    if(Q.size() == 0) return true; // If Q is empty return true.
    cout << "Q size is " << Q.size() << endl;
    std::sort(Q.begin(), Q.end(), tuple_compare);
    int j, i, c, k = 0; 
    for(k = 0; k < Q.size(); k++) cout << get<0>(Q[k]) << " " << get<1>(Q[k]) << endl; // cout << "Created Q. Now calling generatesched " << Q.size() << endl;
    
    int t = get<0>(Q[0]), w = get<1>(Q[0]); // Store the lexicographically minimum <t, w> pair. where t is the team id and w is the week id or run id (both 1-indexed).
    cout << "t and w are " << t << " " << w << endl;
    int *choices = (int *)malloc((2 * n - 2) * sizeof(int)); // Do not use choices[0]
    int n_2 = 2 * n;

    i = 0;
    c = 1; // Populate choices.
    while(i < (n_2 - 2)){
        if(c != t){
           choices[i++] = c;
           choices[i++] = -c;
        }
        c++;
    }
    
    cout << "Choices. " << endl;
    for(i = 0; i < (n_2 - 2); i++) cout << choices[i] << " ";
    cout << endl;
    //cout << std::uniform_int_distribution<int>(0,2000) << endl; 
    // srand ( unsigned ( std::uniform_int_distribution<int>(0,2000) ) );
    random_shuffle(&choices[0], &choices[n_2 - 3], myrandom); // Shuffle choices
    cout << "Shuffled choices. " << endl;
    for(i = 0; i < (n_2 - 2); i++) cout << choices[i] << " ";
    cout << endl;
    
    for(i = 0; i < (n_2 - 2); i++){
        int o = choices[i];
        cout << "Checking if o is free: " << o << endl;
        //Check if team o is free and can be assigned to a game in this week (w)
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
         cout << "Free team o found for team t in week w: " << o << " " << t << " " << w << endl;

        cout << "S before allotment: " << endl;
        nttsa::display_S(S, n, runs);
        S[w + abs(t) * runs] = o;
        cout << "Now 0 " << endl;
        nttsa::display_S(S, n, runs);
        if(o > 0){
            S[w + o * runs] = -t;
            cout << "Now 1" << endl;
            nttsa::display_S(S, n, runs);
        }
        else{
            int o_ = -1 * o;
            S[w + o_ * runs] = t;
            cout << "Now 2" << endl;
            nttsa::display_S(S, n, runs);
        }
        cout << "S after allotment: " << endl;
        nttsa::display_S(S, n, runs);
        remove_from_vector(Q, make_tuple(t, w)); // Remove this tuple from the vector
        remove_from_vector(Q, make_tuple(abs(o), w));

        if(generateSchedule(Q, S)){
            free(choices);
            return true;
        }
        break;
    }/*
        //tuple<int, int> o_w = make_tuple(abs(o), w);
        //vector<tuple<int, int> >::iterator finder = find(Q.begin(), Q.end(), o_w);
        cout << "Loop with t w o " << t << " " << w << " " << o << endl;
        // cout << "Finder returned " << o << " " << w << endl;
        if(finder != Q.end()){ // Element <o, w> not in Q
            cout << "And not found" << endl;
            // Check to see if the selected opponent o is not already assigned in week w,
            isPresent = false;
            for(j = 1; j <= runs; j++)
            if(S[j + abs(t) * n] == o){
                isPresent = true;
                break;
            }

            if(!isPresent)
                S[w + abs(t) * n] = o; // S[t, w] = o
            else continue;
            
            if(o > 0){ // If it is team o's home then assign team t as away in the schedule of team o
                isPresent = false;
                for(j = 1; j <= runs; j++)
                if(S[j + o * n] == (-1 * t)){ // team t has already been assigned in o's schedule as away in some round.
                    isPresent = true;
                    break;
                }

                if(!isPresent)
                    S[w + o * n] = -t;
                else continue;
            } // if(o > 0) ends
            else{ // If it is team o's away, then assign t in o's schedule as home
                isPresent = false;
                for(j = 1; j <= runs; j++)
                if(S[j + abs(o) * n] == t){ // Team t has already been assigned in some other slot to play with o. Skip assigning again.
                    isPresent = true;
                    break;
                }
                
                if(!isPresent)
                    S[w + (abs(o) * n)] = t; // If t is playing with o at o's home, then S[o, w] must be positive.
                else continue;
            }

            // If the team o was alloted in team t's schedule (iff it wasn't already present) recursively try other combinations.
            
            // cout << "Q before " << Q.size() << endl;       
            remove_from_vector(Q, make_tuple(t, w)); // Remove this tuple from the vector
            remove_from_vector(Q, make_tuple(abs(o), w));
            cout << "Removed (" << t << "," << w << ") and (" << abs(o) << "," << w << ")." << endl;
            //remove_from_vector(Q, make_tuple(-o, w));
            cout << "Q after removal " << Q.size() << endl;
            k = 0;
            for(k = 0; k < Q.size(); k++){
                cout << get<0>(Q[k]) << " " << get<1>(Q[k]) << endl; // cout << "Created Q. Now calling generatesched " << Q.size() << endl;
            }
            // exit(0);
            if(generateSchedule(Q, S)){
                free(choices);
                return true;
            } // End generateSch.. if
        } // finder if ends
    } // Main for ends
*/
    cout << "Breaking for t w " << t << " " << w << endl;
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
