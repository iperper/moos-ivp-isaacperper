/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-..-.-.

* FILE: Point.h

* ORIGIN: Department of Mechanical Eng / CSAIL, MIT Cambridge MA

* CREATED: 25 Jul 2018

* MODIFIED: Wed 25 Jul 2018 10:57:37 AM EDT

* CREATED BY: Isaac Perper 

_._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._.*/
#ifndef POINT_HEADER
#define POINT_HEADER

#include <vector>
#include "MOOS/libMOOS/MOOSLib.h"
#include <string>
#include "MBUtils.h"

using namespace std;

struct Point {
     Point(){x_coord=0; y_coord=0; id=-1;}

     Point(string str);
     string to_string();    
    
     void setVals(double x, double y, int id_n){
      x_coord = x;
      y_coord = y;
      id      = id_n; 
     }
     
     bool operator< (const Point & other)
     {
       return x_coord < other.x_coord;
     }



     double x_coord;
     double y_coord;
     int id;
};

#endif
