/* 
 * Unit tests on individual funtions and utilities.
 */

#include <iostream>

#include "ttsa.h"

using namespace std;

void test_randomSchedule(nttsa::TTSA &t_obj){

    t_obj.randomSchedule();

    cout << "S matrix after initialization is " << endl;
    nttsa::display_S(t_obj.get_S(), t_obj.get_n(), t_obj.get_runs());
}

int main(int argc, char *argv[]){
    
    nttsa::TTSA t_obj;
    cout << "Object created " << endl;
    test_randomSchedule(t_obj);
    return 0;
}
