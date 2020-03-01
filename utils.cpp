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
            std::cout << S[j + i * n] << "    ";
        std::cout << std::endl;
    }
}

void nttsa::remove_from_vector(std::vector<std::tuple<int, int> > & Q, std::tuple<int, int> pair){
    
    std::vector<std::tuple<int, int> >::iterator finder = std::find(Q.begin(), Q.end(), pair);

    if(finder != Q.end()) // Element found
        Q.erase(Q.begin() + std::distance(Q.begin(), finder));

}

// template int nttsa::remove_from_vector<std::tuple<int, int> >(std::vector<std::tuple<int, int> > &, std::tuple<int, int>);
