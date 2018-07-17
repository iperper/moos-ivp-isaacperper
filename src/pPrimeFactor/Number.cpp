/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-..-.-.

* FILE: Number.cpp

* ORIGIN: Department of Mechanical Eng / CSAIL, MIT Cambridge MA

* CREATED: 02 Jul 2018

* MODIFIED: Thu 05 Jul 2018 11:47:27 AM EDT

* CREATED BY: Isaac Perper 

_._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._.*/
#include "Number.h"
#include <stdio.h>
#include <sstream>
#include <iostream>
#include <cmath>

using namespace std;

Number::Number(string val, unsigned int rec, double time)
{
  init_num = strtoul(val.c_str(), NULL, 0);
  current_factor = 2;
  current_num = init_num;
  received = rec;
  start_time = time;
}
  
bool Number::findPrimes()
{
  if (current_num < current_factor){
    //cout << "\n Curr num < curr factor \n\n" << endl;
    if (init_num == 1){
      prime_factors.push_back(init_num);
    }
    return (true);
  }

  if (current_factor > sqrt(init_num)){
      prime_factors.push_back(init_num);
      return (true);
  }
  
  else if (current_num%current_factor == 0){
    current_num /= current_factor;
    prime_factors.push_back(current_factor);
  }

  else{
    current_factor++;
  }
  //cout << "\n Returning false \n\n" << endl; 
  return (false);
}

char* Number::outPrimes(unsigned int calc, double time)
{

  stringstream ss;
  ss << prime_factors[0];
  string prime_str = ss.str();
  ss.str("");
  
  for (unsigned int i = 1; i<prime_factors.size(); i++){
    ss << prime_factors[i];
    prime_str += ":";
    prime_str += ss.str();
    ss.str("");
  }
  char* buffer = new char[150];
  sprintf(buffer, "orig=%lu,received=%d,calculated=%d,solve_time=%f,primes=%s,username=iperper",init_num, received, calc, time-start_time,prime_str.c_str());

  return buffer;

}

