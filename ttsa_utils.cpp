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

    int i, j, k = 0;

    for(i = 1; i <= this->n; i++)
    for(j = 1; j <= this->runs; j++)
        Q.push_back(make_tuple(i, j));
    
    // std::sort(Q.begin(), Q.end(), tuple_compare);
    
    /*for(i = 1; i <= this->n; i++)
    for(j = 1; j <= this->runs; j++){
        cout << get<0>(Q[k]) << " " << get<1>(Q[k]) << endl; // cout << "Created Q. Now calling generatesched " << Q.size() << endl;
        k++;
    }*/
    
    srand ( unsigned ( std::time(0) ) );
    
    this->generateSchedule(Q, S);
}

bool nttsa::TTSA::generateSchedule(vector<tuple<int, int> > Q, int *S){
/*
 * Generate (recursively) the initial schedule S.
 */
    // cout << "Inside GS " << Q.size() << endl;
    
    if(Q.size() == 0) return true; // If Q is empty return true.
    
    // cout << "Q size is " << Q.size() << endl;
    
    int o, j, i, c, k = 0; 
   
    // for(k = 0; k < Q.size(); k++) cout << get<0>(Q[k]) << " " << get<1>(Q[k]) << endl; // cout << "Created Q. Now calling generatesched " << Q.size() << endl;
    
    int t = get<0>(Q[0]), w = get<1>(Q[0]); // Store the lexicographically minimum <t, w> pair. where t is the team id and w is the week id or run id (both 1-indexed).
    //cout << "t and w are " << t << " " << w << endl;
    int *choices = (int *)malloc((2 * n - 2) * sizeof(int)); // Create array for choices
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
    
    /*
    cout << "Choices. " << endl;
    for(i = 0; i < (n_2 - 2); i++) cout << choices[i] << " ";
    cout << endl;
    cout << std::uniform_int_distribution<int>(0,2000) << endl;
    */

    random_shuffle(&choices[0], &choices[n_2 - 3], myrandom); // Shuffle choices
    /*
    cout << "Shuffled choices. " << endl;
    for(i = 0; i < (n_2 - 2); i++) cout << choices[i] << " ";
    cout << endl;
    */

    for(i = 0; i < (n_2 - 2); i++){
        o = choices[i];
        // cout << "Checking if o is free: " << o << endl;
        
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
         // cout << "Free team o found for team t in week w: " << o << " " << t << " " << w << endl;

        /*
        cout << "S before allotment: " << endl;
        nttsa::display_S(S, n, runs);
        */
        
        S[w + abs(t) * runs] = o;
        
        /*
        cout << "Now 0 " << endl;
        nttsa::display_S(S, n, runs);
        */
        if(o > 0){
            S[w + o * runs] = -t;
        }
        else{
            int o_ = -1 * o;
            S[w + o_ * runs] = t;
        }
        /*
        cout << "S after allotment: " << endl;
        nttsa::display_S(S, n, runs);
        */
        remove_from_vector(Q, make_tuple(t, w)); // Remove this tuple from the vector
        remove_from_vector(Q, make_tuple(abs(o), w));

        if(generateSchedule(Q, S)){
            free(choices);
            return true;
        }
        break;
    }
        
    // cout << "Breaking for t w " << t << " " << w << endl;
    free(choices);
    return false;
}

void nttsa::TTSA::init_S_from_file(ifstream &fin){
    int i, j;
    char ch;

    for(i = 1; i <= n; i++)
    for(j = 1; j <= runs && fin.get(ch); j++){
        if(ch == '-'){
            fin.get(ch);
            S[j + i * runs] = -1 * atoi(&ch);
        }
        else S[j + i * runs] = atoi(&ch);
        fin.get(ch); // read newline or space
    }
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
