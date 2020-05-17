/*
 * Non-TTSA specific utilities
 */

#include <vector>
#include <tuple>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <random>
#include <ctime>
#include <chrono>

#include "ttsa.h"

using namespace std::chrono;

// Source: https://stackoverflow.com/a/49360677/10182805
double nttsa::what_time_is_it()
{
      struct timespec now;
           clock_gettime(CLOCK_REALTIME, &now);
                return now.tv_sec + now.tv_nsec*1e-9;
}


void nttsa::display_S(int *S, int n, int runs){

    int i, j;

    for(i = 1; i <= n; i++){
        for(j = 1; j <= runs; j++)
            std::cout << S[j + i * runs] << "    ";
        std::cout << std::endl;
    }
}

bool nttsa::sample_prob(float temp, float delta){
   
   // Source: https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution
   std::random_device seeder;
   std::mt19937 gen(seeder()); //Standard mersenne_twister_engine seeded with rd()
   std::uniform_real_distribution<> sampler(0.0, 1.0);

   if(sampler(gen) < std::exp(-1.0 * delta/temp)) return true;

   return false;
}

float nttsa::f_func(int inp){
/*
 * Computes the sub-linear function f(v) = 1 + sqrt(v) * ln(v) / 2
 */

    return (1 + (sqrt(inp) * std::log(inp) / 2.0));
}

void nttsa::display_D(int *d, int n){

    int i, j;

    for(i = 1; i <= n; i++){
        for(j = 1; j <= n; j++)
            std::cout << d[j + i * n] << "   ";
        std::cout << std::endl;
    }
}

void nttsa::remove_from_vector(std::vector<std::tuple<int, int> > & Q, std::tuple<int, int> pair){
   int i;
   
   std::vector<std::tuple<int, int> >::iterator finder = std::find(Q.begin(), Q.end(), pair);
   
   if(finder != Q.end()) // Element found
        Q.erase(Q.begin() + std::distance(Q.begin(), finder));
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
