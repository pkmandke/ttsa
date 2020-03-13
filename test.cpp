/* 
 * Unit tests on individual funtions and utilities.
 */

#include <iostream>
#include <fstream>
#include <cstdbool>
#include <random>

#include "ttsa.h"

using namespace std;

void test_randomSchedule(nttsa::TTSA &t_obj){

    t_obj.randomSchedule();

    cout << "S matrix after random initialization is " << endl;
    nttsa::display_S(t_obj.get_S(), t_obj.get_n(), t_obj.get_runs());

    cout << "Is schedule RR> => " << t_obj.verifyRR(t_obj.get_S()) << endl;
}

void re_init(int *Sch, int *S, int n, int runs){

    int i, j;
    for(i = 1; i <= n; i++)
    for(j = 1; j <= runs; j++) Sch[j + runs * i] = S[j + runs * i];
}

void test_nbd_utils(nttsa::TTSA &t_obj){
          
    //t_obj.randomSchedule();
    // cout << "Original Schedule." << endl;
    // nttsa::display_S(t_obj.get_S(), t_obj.get_n(), t_obj.get_runs());

    int *Sch = (int *)malloc((t_obj.get_n() + 1) * (t_obj.get_runs() + 1) * sizeof(int));
    int *S = t_obj.get_S();
    while(!t_obj.randomSchedule());
    re_init(Sch, S, t_obj.get_n(), t_obj.get_runs());

    int s1, s2, s3, s4;

    cout << "Original Schedule." << endl;
    nttsa::display_S(t_obj.get_S(), t_obj.get_n(), t_obj.get_runs());

    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> dis(1, t_obj.get_n()); // 5 moves in total


    std::random_device rd1;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen1(rd1()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> dis1(1, t_obj.get_runs()); // 5 moves in total

    // t_obj.swapHomes(Sch, dis(gen), dis(gen));
    do{
    s1 = dis1(gen1);
    s2 = dis1(gen1);
    s3 = dis(gen);
    s4 = dis(gen);
    }while(s3 == s4 || s1 == s2);
    // cout << "Swapping partial rounds r1, r2, t = " << s1 << ", " << s2 << ", " << s3 << endl;
    // t_obj.swapRounds(Sch, s1, s2);
    //t_obj.partialSwapRounds(Sch, s3, s1, s2);
    cout << "Partial swap teams t1, t2, r = " << s3 << ", " << s4 << ", " << s2 << endl;
    while(!t_obj.partialSwapTeams(Sch, s3, s4, s2)){ 
        do{
            s1 = dis1(gen1);
            s2 = dis1(gen1);
            s3 = dis(gen);
            s4 = dis(gen);
        }while(s3 == s4 || s1 == s2);
        cout << "Partial swap teams t1, t2, r = " << s3 << ", " << s4 << ", " << s2 << endl;
    }
    if(t_obj.verifyRR(Sch)) cout << " RR is true." << endl;
    else cout << "RR is false!" << endl;
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


void test_cost(nttsa::TTSA &t){
    test_randomSchedule(t);
    // test_init_D(t); // Uncomment this line!!!
    cout << "Cost = " << t.get_cost(t.get_S()) << std::endl;
}

void test_nbv(nttsa::TTSA &t){
    test_randomSchedule(t);
    cout << "NBV = " << t.nbv(t.get_S()) << endl;;
}

void test_core_funcs(nttsa::TTSA &t){
    
    cout << "S before applying move" << endl;
    nttsa::display_S(t.get_S(), t.get_n(), t.get_runs());
    
    while(!t.randomSchedule());
    t.apply_random_move(t.get_S());


    cout << "S before after move" << endl;
    nttsa::display_S(t.get_S(), t.get_n(), t.get_runs());
}


void test_core_SA(nttsa::TTSA &t){


    
    while(!t.randomSchedule());
   
   cout << "S before train:" << endl;
   nttsa::display_S(t.get_S(), t.get_n(), t.get_runs());
   
   t.train(5, 5, 5, 5, 0.4, 2, 2, 6);

    cout << "S after train" << endl;
    nttsa::display_S(t.get_S(), t.get_n(), t.get_runs());
    cout << " Verify RR says " << t.verifyRR(t.get_S()) << endl;

    cout << "FInal cost is " << t.get_cost(t.get_S()) << endl;
    cout << "Final is feasible? => " << t.isFeasible(t.get_S()) << endl;
    cout << "Final nbv = " << t.nbv(t.get_S()) << endl;
}

void test_init_D(nttsa::TTSA &t_obj){
    
    int i;
    ifstream f;
        f.open("./data/nl_6.txt");

        t_obj.init_D_from_file(f);
        cout << "Distance matrix is: " << std::endl;
    
        nttsa::display_D(t_obj.get_dist(), t_obj.get_n());
        f.close();
}


int main(int argc, char *argv[]){
    
    nttsa::TTSA t_obj(atoi(argv[1]), 1);
    cout << "Object created " << endl;
    test_randomSchedule(t_obj);
    
    //ifstream f;
    // f.open("./schedules/sched_pSwaprounds.txt");
    // t_obj.init_S_from_file(f);
    // nttsa::display_S(t_obj.get_S(), t_obj.get_n(), t_obj.get_runs());
    //test_nbd_utils(t_obj);
    // test_cost(t_obj);
    // test_init_D(t_obj);
    // test_constraints(t_obj);
    // test_nbv(t_obj);
    //
    
   // test_init_D(t_obj);
    int i;
    //for(i = 0; i < 10; i++) test_nbd_utils(t_obj);
   // test_core_SA(t_obj);
    //for(i = 0; i < 10; i++) test_core_funcs(t_obj);
    //
    // test_core_SA(t_obj);
    return 0;
}
