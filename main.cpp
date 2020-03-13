/*
 * The main program that instantiates the TTSA class and runs multiple iterations of the algorithm for benchmarking.
 */

#include <iostream>
#include <fstream>

#include "ttsa.h"

using namespace std;

int main(int argc, char *argv[]){
    
    int maxc = 5000, maxp = 7100, maxr = 10, w0 = 4000, t0 = 400, n = 16;

    float beta = 0.9999, delta = 1.04, theta = 1.04;

    nttsa::TTSA t(n = n);
    
    ofstream fout;
    ifstream f;
    f.open("./data/nl_16.txt");
    t.init_D_from_file(f);

    while(!t.randomSchedule());
    
    cout << "Initial (random) schedule is: " << endl;
    nttsa::display_S(t.get_S(), t.get_n(), t.get_runs());

    cout << "Distance matrix: " << endl;
    nttsa::display_D(t.get_dist(), t.get_n());

    t.train(maxr, maxp, maxc, t0, beta, delta, theta, w0);

    cout << "Final schedule is: " << endl;
    nttsa::display_S(t.get_S(), t.get_n(), t.get_runs());

    cout << "Cost or distance D = " << t.get_distance(t.get_S()) << endl;
    
    cout << "Is schedule feasible? => " << t.isFeasible(t.get_S()) << endl;

    cout << "Num of nbv violations " << t.nbv(t.get_S()) << endl;

    return 0;
}
