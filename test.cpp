/* 
 * Unit tests on individual funtions and utilities.
 */

#include <iostream>
#include <fstream>

#include "ttsa.h"

using namespace std;

void test_randomSchedule(nttsa::TTSA &t_obj){

    t_obj.randomSchedule();

    cout << "S matrix after initialization is " << endl;
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

int main(int argc, char *argv[]){
    
    nttsa::TTSA t_obj(6, 1);
    cout << "Object created " << endl;
    // test_randomSchedule(t_obj);
    
    ifstream f;
    f.open("./schedules/sched_pSwaprounds.txt");
    t_obj.init_S_from_file(f);
    nttsa::display_S(t_obj.get_S(), t_obj.get_n(), t_obj.get_runs());
    test_nbd_utils(t_obj);
    return 0;
}
