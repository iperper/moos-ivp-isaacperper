/************************************************************/
/*    NAME: Isaac Perper                                              */
/*    ORGN: MIT                                             */
/*    FILE: PointAssign.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef PointAssign_HEADER
#define PointAssign_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include "MOOS/libMOOS/MOOSLib.h"
#include "XYPoint.h"
#include "XYFormatUtilsPoint.h"
#include <string>
#include <vector>
#include "Point.h"

using namespace std;

class PointAssign : public AppCastingMOOSApp
{
 public:
   PointAssign();
   ~PointAssign();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();
   void postViewPoint(double x, double y, string label, string color);
   bool split_region();
   bool split_alternating();

 protected:
   void registerVariables();
  
 private: // Configuration variables
   vector<string> m_vehicles;
   bool m_assign_by_region;

 private: // State variables 
   vector<Point> m_points;
   bool m_add_points;
   string m_curr_input;
   int m_good_messages;
   bool m_points_sent;
};

#endif 
