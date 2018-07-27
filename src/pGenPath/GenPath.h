/************************************************************/
/*    NAME: Isaac Perper                                              */
/*    ORGN: MIT                                             */
/*    FILE: GenPath.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef GenPath_HEADER
#define GenPath_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include <vector>
#include <string>
#include <cmath>
#include "XYSegList.h"
#include "Point.h"

using namespace std;

class GenPath : public AppCastingMOOSApp
{
 public:
   GenPath();
   ~GenPath();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();

 protected:
   void registerVariables();

 private: // Configuration variables
   double m_visit_radius;

 private: // State variables
   bool m_receiving_points;
   int  m_good_messages;
   int  m_found_points;
   bool m_received_points;
   bool m_points_configured;
   bool m_points_sent;
   int m_points_reached;

    bool m_first_pass_done;

   vector<Point> m_points;
   Point m_current;
   XYSegList m_seglist;
   bool extra_done;

   double m_nav_x;
   double m_nav_y;
   string m_val;
  
 protected:
   unsigned int getClosest(Point &curr, set<unsigned int> added);
};

#endif 
