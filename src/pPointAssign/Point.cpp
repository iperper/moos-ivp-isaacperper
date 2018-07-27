/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-..-.-.

* FILE: Point.cpp

* ORIGIN: Department of Mechanical Eng / CSAIL, MIT Cambridge MA

* CREATED: 25 Jul 2018

* MODIFIED: Wed 25 Jul 2018 10:53:20 AM EDT

* CREATED BY: Isaac Perper 

_._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._.*/

#include "Point.h"

Point::Point(string str)
{
  vector<string> vals = parseString(str, ",");
  for (unsigned int i=0; i<vals.size(); i++){
    biteString(vals[i], '=');
  }
  x_coord = strtod(vals[0].c_str(), NULL);
  y_coord = strtod(vals[1].c_str(), NULL);
  id = (int) strtod(vals[2].c_str(), NULL);
}

string Point::to_string()
{
  string out = "x="+doubleToString(x_coord, 3)+" ,y="+doubleToString(y_coord, 3)+" ,id="+intToString(id);
  return out;
}



