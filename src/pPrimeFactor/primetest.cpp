/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-..-.-.

* FILE: primetest.cpp

* ORIGIN: Department of Mechanical Eng / CSAIL, MIT Cambridge MA

* CREATED: 02 Jul 2018

* MODIFIED: Tue 03 Jul 2018 03:34:33 PM EDT

* CREATED BY: Isaac Perper 

_._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._.*/
#include <cstdint>
#include <cstdlib>
#include <vector>
#include <iostream>
#include "Number.h"
#include <list>

using namespace std;

int main (int argc, char **argv){
  Number val(argv[1]);
 // bool done = false;

  list<Number> val_list {val};

  
  while (val_list.size()){
  list<Number>::iterator it = val_list.begin();
  while (it != val_list.end()){
    cout << "In While" << endl;    
    bool done = it->findPrimes();
    cout << done << endl;
    if (done){
        char* out= it->outPrimes();
        val_list.erase(it++);  // alternatively, i = items.erase(i);
        cout << out << endl;
    }
    
    else{
      it++;
    }

  }
  }

/* 
  while(!done){
    done = val.findPrimes();
  }

  char* out = val.outPrimes();
*/

  return(1);
}



