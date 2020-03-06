/*
 * Non-TTSA specific utilities
 */

#include <vector>
#include <tuple>
#include <iostream>
#include <cstdlib>

#include "ttsa.h"

void nttsa::display_S(int *S, int n, int runs){

    int i, j;

    for(i = 1; i <= n; i++){
        for(j = 1; j <= runs; j++)
            std::cout << S[j + i * runs] << "    ";
        std::cout << std::endl;
    }
}

void nttsa::remove_from_vector(std::vector<std::tuple<int, int> > & Q, std::tuple<int, int> pair){
   int i;
    // std::cout << "Pair is " << std::get<0>(pair) << " " << std::get<1>(pair) << std::endl;
    std::vector<std::tuple<int, int> >::iterator finder = std::find(Q.begin(), Q.end(), pair);
    // for(i = 0; i < Q.size(); i++)
    //    std::cout << "t w " << std::get<0>(Q[i]) << " " << std::get<1>(Q[i]) << std::endl;
    if(finder != Q.end()) // Element found
        Q.erase(Q.begin() + std::distance(Q.begin(), finder));
    // std::cout << "After " << std::endl;
    // for(i = 0; i < Q.size(); i++)
    //    std::cout << "t w " << std::get<0>(Q[i]) << " " << std::get<1>(Q[i]) << std::endl;
   // exit(0);
}

void nttsa::swapInts(int *i, int *j){

    int t;

    t = *i;
    *i = *j;
    *j = t;
}

int nttsa::sign_of(int i){

    if(i >= 0) return 1;
    
    return -1;
}

int nttsa::find_pos(int *arr, int n, int key){
/* 
 * Return the position of key in array.
 * return -1 if key not found.
 */

    int i;

    for(i = 0; i < n; i++)
        if(arr[i] == key) return i;

    return -1;
}
// template int nttsa::remove_from_vector<std::tuple<int, int> >(std::vector<std::tuple<int, int> > &, std::tuple<int, int>);
