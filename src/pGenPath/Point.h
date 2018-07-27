/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-..-.-.

* FILE: Point.h

* ORIGIN: Department of Mechanical Eng / CSAIL, MIT Cambridge MA

* CREATED: 25 Jul 2018

* MODIFIED: Fri 27 Jul 2018 11:43:34 AM EDT

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
     Point(){x_coord=0; y_coord=0; id=-1;found=false;}
     Point(string str);
     ~Point(){}

     string to_string();    
    
     void setVals(double x, double y, int id_n){
      x_coord = x;
      y_coord = y;
      id      = id_n; 
      found = false;
     }
     
     bool operator< (const Point & other)
     {
       return x_coord < other.x_coord;
     }
     
     bool  operator==(const Point &other){
       return (x_coord == other.x_coord &&
           y_coord == other.y_coord &&
           id == other.id);
     }

     bool operator!=(const Point &other){
       return (x_coord != other.x_coord ||
           y_coord != other.y_coord ||
           id != other.id);
     }

     double x_coord;
     double y_coord;
     int id;
     bool found;
};

#endif
