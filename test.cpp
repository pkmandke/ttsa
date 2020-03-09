/* 
 * Unit tests on individual funtions and utilities.
 */

#include <iostream>
#include <fstream>
#include <cstdbool>

#include "ttsa.h"

using namespace std;

void test_randomSchedule(nttsa::TTSA &t_obj){

    t_obj.randomSchedule();

    cout << "S matrix after random initialization is " << endl;
    nttsa::display_S(t_obj.get_S(), t_obj.get_n(), t_obj.get_runs());
}

void re_init(int *Sch, int *S, int n, int runs){

    int i, j;
    for(i = 1; i <= n; i++)
    for(j = 1; j <= runs; j++) Sch[j + runs * i] = S[j + runs * i];
}

void test_nbd_utils(nttsa::TTSA &t_obj){
          
    //t_obj.randomSchedule();
    cout << "Original Schedule." << endl;
    nttsa::display_S(t_obj.get_S(), t_obj.get_n(), t_obj.get_runs());

    int *Sch = (int *)malloc((t_obj.get_n() + 1) * (t_obj.get_runs() + 1) * sizeof(int));
    int *S = t_obj.get_S();

    re_init(Sch, S, t_obj.get_n(), t_obj.get_runs());

    //t_obj.swapHomes(Sch, 1, 2);
    // t_obj.swapRounds(Sch, 2, 5);
    // t_obj.partialSwapRounds(Sch, 2, 2, 9);
    t_obj.partialSwapTeams(Sch, 2, 4, 9);
    cout << "Swapping homes of team 1 and 2" << endl;
    nttsa::display_S(Sch, t_obj.get_n(), t_obj.get_runs());

}

void test_constraints(nttsa::TTSA &t_obj){
    if(t_obj.isAtmost(t_obj.get_S())) cout << "Atmost constraint satisfied." << endl;
    else cout << "Atmost constraint not satisfied." << endl;
    
    if(t_obj.isNorepeat(t_obj.get_S())) cout << "No repeat constraint satisfied." << endl;
    else cout << "No repeat constraint not satisfied." << endl;
    
    if(t_obj.isFeasible(t_obj.get_S())) cout << "Feasible." << endl;
    else cout << "Not feasible." << endl;
}

void test_init_D(nttsa::TTSA &t_obj){
    
    ifstream f;
    f.open("./dist/test_distance_4.txt");

    t_obj.init_D_from_file(f);
    cout << "Distance matrix is: " << std::endl;
    
    nttsa::display_D(t_obj.get_dist(), t_obj.get_n());
}

void test_cost(nttsa::TTSA &t){
    test_randomSchedule(t);
    test_init_D(t);
    cout << "Cost = " << t.get_cost(t.get_S()) << std::endl;
}

void test_nbv(nttsa::TTSA &t){
    test_randomSchedule(t);
    cout << "NBV = " << t.nbv(t.get_S()) << endl;;
}
int main(int argc, char *argv[]){
    
    nttsa::TTSA t_obj(4, 1);
    cout << "Object created " << endl;
    // test_randomSchedule(t_obj);
    
    //ifstream f;
    // f.open("./schedules/sched_pSwaprounds.txt");
    // t_obj.init_S_from_file(f);
    // nttsa::display_S(t_obj.get_S(), t_obj.get_n(), t_obj.get_runs());
    //test_nbd_utils(t_obj);
    // test_cost(t_obj);
    // test_init_D(t_obj);
    // test_constraints(t_obj);
    test_nbv(t_obj);
    return 0;
}
