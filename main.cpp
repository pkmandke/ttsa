/*
 * The main program that instantiates the TTSA class and runs multiple iterations of the algorithm for benchmarking.
 */

#include <iostream>
#include <fstream>
#include <cstdlib>

#include "ttsa.h"

using namespace std;

void basic_tests(int argc, char *argv[]){

    int maxc = 5000, maxp = 7100, maxr = 10, w0 = 4000, t0 = 400, n = 8;

    float beta = 0.9999, delta = 1.04, theta = 1.04;

    nttsa::TTSA t(n = n);
    
    ofstream fout;
    string f_name = "./data/nl_";
    f_name += argv[1];
    f_name += ".txt";
    ifstream f;
    f.open(f_name);
    cout << "Distance fileName: " << f_name << endl;
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

}


void runtime_tests(int argc, char *argv[]){
    
    int maxc = 4000, maxp = 7100, maxr = 30, w0 = 20000, t0 = 600, n = atoi(argv[1]);

    float beta = 0.9999, delta = 1.03, theta = 1.03;

    nttsa::TTSA t(n = n);
    cout << "N = " << n << endl;

    ofstream fout;
    string f_name = "./data/nl_";
    f_name += argv[1];
    f_name += ".txt";
    ifstream f;
    f.open(f_name);
    cout << "Distance fileName: " << f_name << endl;
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

}

int main(int argc, char *argv[]){
    
    // basic_tests(argc, argv);
    runtime_tests(argc, argv);
    return 0;
}
