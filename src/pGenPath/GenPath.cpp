/************************************************************/
/*    NAME: Isaac Perper                                              */
/*    ORGN: MIT                                             */
/*    FILE: GenPath.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "GenPath.h"
#include <iostream>

using namespace std;

//---------------------------------------------------------
// Constructor

GenPath::GenPath()
{
  m_receiving_points = false;
  m_points_configured = false;
  m_received_points = false;
  m_current.setVals(0, 0, 0);
  m_good_messages = 0;
  m_found_points = 0;
  m_visit_radius = 5;
  m_nav_x = 0;
  m_nav_y = 0;

  m_points_sent=false;
  m_points_reached = 0;
  m_first_pass_done = false;
  extra_done = false;
}

//---------------------------------------------------------
// Destructor

GenPath::~GenPath()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool GenPath::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail);

  MOOSMSG_LIST::iterator p;
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    string key    = msg.GetKey();
     
 // UPDATES_VAR depends on your config
#if 0 // Keep these around just for template
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString(); 
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
#endif
    string sval = msg.GetString();

    if (key == "VISIT_POINT"){

      if(sval == "firstpoint"){
        Notify("TEST", "receiving");
        m_receiving_points = true;
      }

      else if(sval == "lastpoint"){
        Notify("TEST", "last point received");
        m_receiving_points = false;
        m_received_points = true;
      }

      else if (m_receiving_points){
        Point pt(sval);
        m_points.push_back(pt);
        m_good_messages++;
        Notify("TEST", "adding points");
      }

    }

    else if (key == "NAV_X"){
      m_nav_x = msg.GetDouble();
    }
    
    else if (key == "NAV_Y"){
      m_nav_y = msg.GetDouble();
    }

    else if (key == "REGEN"){
      Notify("TEST", "received");
      if (sval == "true") m_first_pass_done = true;
    }
  
    else if(key != "APPCAST_REQ") // handled by AppCastingMOOSApp
       reportRunWarning("Unhandled Mail: " + key);
    

  }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool GenPath::OnConnectToServer()
{
   registerVariables();
   return(true);
}


//--------------------------------------------------------
// Procedure: getClosest
//
unsigned int GenPath::getClosest(Point &curr, set<unsigned int> added)
{
  double min_dist = 100000;
  unsigned int min_ind = 0;
  int counter = 0; 
  string out = "Out: ";
  for (unsigned int i=0; i<m_points.size(); i++){
    if (added.find(i) == added.end() && !m_points[i].found){
      counter++;
      out += " "+intToString(i);
      double temp_dist = hypot(curr.x_coord-m_points[i].x_coord, curr.y_coord-m_points[i].y_coord);
      if (temp_dist <= min_dist){
        min_dist = temp_dist;
        min_ind = i;
      }
    }
  }
//  Notify("TEST", out);
//  Notify("TEST", "Counter: " + intToString(counter));
  return (min_ind);
}


//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second



bool GenPath::Iterate()
{
  AppCastingMOOSApp::Iterate();
    

  if (!m_points_configured && m_received_points){
      set<unsigned int> added;
      unsigned int j;
      int count = 0;
      for (unsigned int i=0; i<m_points.size()-m_found_points; i++){
        j = getClosest(m_current,added);
        added.insert(j);
        m_current=m_points[j];
        m_seglist.add_vertex(m_points[j].x_coord, m_points[j].y_coord);
        count++;
      }
      Notify("TEST3", "seg count: " + intToString(count));
      m_points_configured = true;
  }

  if (m_points_configured && !m_points_sent){
    string update_str = "points = ";
    update_str      += m_seglist.get_spec();
    Notify("WAYPT_UPDATE", update_str);
    Notify("POINTS_IN", "true");
    m_points_sent = true;
  }
  

  if (m_first_pass_done){
    int count = 0;
    for (unsigned int i=0; i<m_points.size(); i++){
      count += m_points[i].found;
    }
    Notify("TEST2", "count : " + intToString(count));
    m_points_sent=false;
    m_seglist.clear();
    m_current.setVals(m_nav_x, m_nav_y, 0);
    m_points_configured=false;
    m_first_pass_done = false;
    m_received_points = true;
  }

  if (m_points_sent){
    for (unsigned int i=0; i<m_points.size(); i++){
      if (hypot(m_points[i].x_coord-m_nav_x, m_points[i].y_coord-m_nav_y) <= m_visit_radius){
        if(!m_points[i].found){
          m_points[i].found = true;
          m_found_points++;
          m_points_reached++;
        }
        Notify("REACHED", intToString(m_points_reached));
      }
    }
  }




  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool GenPath::OnStartUp()
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
    if(param == "foo") {
      handled = true;
    }
    else if(param == "bar") {
      handled = true;
    }

    if(!handled)
      reportUnhandledConfigWarning(orig);

  }
  
  registerVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: registerVariables

void GenPath::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  // Register("FOOBAR", 0);
  Register("VISIT_POINT", 0);
  Register("NAV_X", 0);
  Register("NAV_Y", 0);
  Register("REGEN", 0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool GenPath::buildReport() 
{
  m_msgs << "good messages: " << m_good_messages << endl;
  m_msgs << "Points size: " << m_points.size() << endl;
  m_msgs << "found points: " << intToString(m_found_points) << endl;
  m_msgs << "Received points: " << m_received_points << endl;
  m_msgs << "Configured: " << m_points_configured << endl;
  m_msgs << "Seglist: " << m_seglist.get_spec() << endl;  
  m_msgs << "extra done: " << extra_done << endl;
  return(true);
}




