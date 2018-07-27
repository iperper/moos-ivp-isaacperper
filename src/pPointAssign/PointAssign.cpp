/************************************************************/
/*    NAME: Isaac Perper                                              */
/*    ORGN: MIT                                             */
/*    FILE: PointAssign.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "PointAssign.h"
#include <algorithm>
#include <stdlib.h>

using namespace std;

//---------------------------------------------------------
// Constructor

PointAssign::PointAssign()
{ 
  m_assign_by_region=false;
  m_add_points = false;
  m_curr_input = "";
  m_good_messages = 0;
  m_points_sent = false;
}

//---------------------------------------------------------
// Destructor

PointAssign::~PointAssign()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool PointAssign::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail);

  MOOSMSG_LIST::iterator p;
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    string key    = msg.GetKey();
    string sval = msg.GetString();

    if (key == "VISIT_POINT"){
      if (sval == "firstpoint"){
        m_add_points = true;
      }
      
      else if (sval == "lastpoint"){
        m_add_points = false;
      }

      else if(m_add_points){
        m_curr_input = sval;
        Point pt(sval);
        m_points.push_back(pt);
        m_good_messages++;
      }
    }


#if 0 // Keep these around just for template
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString(); 
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
#endif

     else if(key != "APPCAST_REQ") // handled by AppCastingMOOSApp
       reportRunWarning("Unhandled Mail: " + key);
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool PointAssign::OnConnectToServer()
{
   registerVariables();
   return(true);
}

/**
// Fucntion: by_x
// Compares the values of two x coordinates for VISIT_POINT
struct PointAssign::sort_by_x {
   bool operator()(string const &a, string const &b) const {
     vector<string> a_x = parseString(a, ",");
     biteString(a_x[0],"=");
     double a_x_d = stdtod(a_x[0], 0);
     vector<string> b_x = parseString(b, ",");
     biteString(b_x[0],"=");
     double b_x_d = stdtod(b_x[0], 0);

     return a_x_d < b_x_d;
   }
}; **/

//Utility Function: postViewPoint
//
void PointAssign::postViewPoint(double x, double y, string label, string color)
 {
   XYPoint point(x, y);
   point.set_label(label);
   point.set_color("vertex", color);
   point.set_param("vertex_size", "2");

   string spec = point.get_spec();
   Notify("VIEW_POINT", spec);
 }

//Function: split_region
//
bool PointAssign::split_region()
{
  //First sort the list by x-coordinates
  sort(m_points.begin(), m_points.end());  

  int curr_vehicle = 0;
  
  for (unsigned int i=0; i<m_points.size()/2; i++){
    Notify("VISIT_POINT_"+m_vehicles[curr_vehicle], m_points[i].to_string());
    PointAssign::postViewPoint(m_points[i].x_coord, m_points[i].y_coord, intToString(m_points[i].id), "yellow");
  }

  ++curr_vehicle;

  for (unsigned int i=m_points.size()/2; i<m_points.size(); i++){
    Notify("VISIT_POINT_"+m_vehicles[curr_vehicle], m_points[i].to_string());
    PointAssign::postViewPoint(m_points[i].x_coord, m_points[i].y_coord, intToString(m_points[i].id), "red");
  }

  return (true);
}

//Function: split_alternation
//
bool PointAssign::split_alternating()
{
  int num_vehicles = m_vehicles.size();
  int curr_vehicle = 0;
  for (unsigned int i=0; i<m_points.size(); i++){
    Notify("VISIT_POINT_"+m_vehicles[curr_vehicle], m_points[i].to_string());
    string color;
    if (curr_vehicle==0){
      color = "yellow";
    }
    else if (curr_vehicle==1){
      color = "red";
    }

    PointAssign::postViewPoint(m_points[i].x_coord, m_points[i].y_coord, intToString(m_points[i].id), color);
    curr_vehicle++;
    if (curr_vehicle==num_vehicles) curr_vehicle=0;
  }

  return (true);
}



//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool PointAssign::Iterate()
{
  AppCastingMOOSApp::Iterate();
  
  if (m_points.size() == 100 && !m_add_points && !m_points_sent){
    for (unsigned int i=0; i<m_vehicles.size(); i++){
      Notify("VISIT_POINT_"+m_vehicles[i], "firstpoint");
    }
    
    if (m_assign_by_region){
      m_points_sent = PointAssign::split_region();
    }else m_points_sent = PointAssign::split_alternating();

    for (unsigned int i=0; i<m_vehicles.size(); i++){
      Notify("VISIT_POINT_"+m_vehicles[i], "lastpoint");
    }

  }
 
  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open
bool PointAssign::OnStartUp()
{
  AppCastingMOOSApp::OnStartUp();

  STRING_LIST sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(!m_MissionReader.GetConfiguration(GetAppName(), sParams))
    reportConfigWarning("No config block found for " + GetAppName());

  STRING_LIST::iterator p;
  for(p=sParams.begin(); p!=sParams.end(); p++) {
    string orig  = *p;
    string line  = *p;
    string param = tolower(biteStringX(line, '='));
    string value = line;
    
      bool handled = false;
     
      if(param == "vname") {
        m_vehicles.push_back(toupper(value));
        handled = true;  
      }

      if(param == "assign_by_region"){
        if (value=="true"){
            m_assign_by_region=true;
            handled = true;
        }
        else{
          m_assign_by_region=false;
          handled = true;
        }
      }
    
      if(!handled)
        reportUnhandledConfigWarning(orig);
  }
  
  Notify("UTS_PAUSE", "false"); 
  registerVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: registerVariables

void PointAssign::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  // Register("FOOBAR", 0);
  Register("VISIT_POINT", 0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool PointAssign::buildReport() 
{
  m_msgs << "VISIT_POINT: " << m_curr_input << endl;
  m_msgs << "Good messages: " << m_good_messages << endl;

  return(true);
}




