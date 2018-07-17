/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-..-.-.

* FILE: Number.h 

* ORIGIN: Department of Mechanical Eng / CSAIL, MIT Cambridge MA

* CREATED: 02 Jul 2018

* MODIFIED: Thu 05 Jul 2018 11:47:28 AM EDT

* CREATED BY: Isaac Perper 

_._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._.*/

#ifndef INPUTNUMBERS
#define INPUTNUMBERS

#include <vector>
#include <cstdint>
#include <cstdlib>
#include <string>

class Number
{
  public:
    Number(std::string val, unsigned int rec, double time);
    ~Number(){};
    uint64_t getInit() const {return init_num;};
    bool findPrimes();
    std::vector<uint64_t> getPrimes() {return prime_factors;};
    char* outPrimes(unsigned int calc, double time);

  protected:
    uint64_t init_num = -1;
    std::vector<uint64_t> prime_factors;
    uint64_t current_factor = -1;
    uint64_t current_num = -1;
    unsigned int received = -1;
    double start_time = -1;
};


#endif
