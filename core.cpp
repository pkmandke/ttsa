/*
 * The core TTSA algorithm.
 */

#include <iostream>
#include <cstdlib>

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
}

void nttsa::TTSA::reset_S(){
    
    int i = 1, j = 1;

    for(;i <= n; i++)
    for(;j <= runs; j++)
        S[j + i * runs] = 0;
}

void nttsa::TTSA::allocate_mem(){
/*
 * Allocate required memory dynamic.
 */

    this->S = (int *)malloc((n + 1) * (runs + 1) * sizeof(int)); // Allocate 1 row and column extra to allow 1 indexing.
}

nttsa::TTSA::~TTSA(){

    free(this->S);
}
