/************************************************************/
/*    NAME: Isaac Perper                                              */
/*    ORGN: MIT                                             */
/*    FILE: BatteryMonitor.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "BatteryMonitor.h"
#include <iostream> 

using namespace std;

//---------------------------------------------------------
// Constructor

BatteryMonitor::BatteryMonitor()
{
  m_current = 0.0;
  m_voltage = 0.0;
  m_mAh = 0.0;
  m_percent_remaining = 100.0;
  m_status = "OK"; 
}

//---------------------------------------------------------
// Destructor

BatteryMonitor::~BatteryMonitor()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool BatteryMonitor::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail);
  MOOSMSG_LIST::iterator p;
   
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    
    string key = msg.GetKey();
 
    if(key == "BATTERY_NMEA_STRING") {
      string nmea = msg.GetString();
      // TODO: check checksum
      vector<string> parsed = parseString(nmea, "|");
      cout << parsed[0] << endl;
      handleBattery(parsed);
    }
  } 
 
#if 0 // Keep these around just for template
    string key   = msg.GetKey();
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString(); 
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
#endif

   return(true);
}

bool BatteryMonitor::handleBattery(vector<string> &parsed){
  m_current = strtod(parsed[1].c_str(),0); 
  m_voltage = strtod(parsed[2].c_str(),0); 
  m_mAh = strtod(parsed[3].c_str(),0); 
  m_percent_remaining = strtod(parsed[4].c_str(),0); 
  m_status = parsed[5];

  Notify("BATT_CURRENT", m_current);
  Notify("BATT_VOLTAGE", m_voltage);
  Notify("BATT_mAH", m_mAh, m_percent_remaining);
  Notify("BATT_%_REMAINING", m_percent_remaining);
  Notify("BATT_STATUS", m_status);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool BatteryMonitor::OnConnectToServer()
{
   // register for variables here
   // possibly look at the mission file?
   // m_MissionReader.GetConfigurationParam("Name", <string>);
   // m_Comms.Register("VARNAME", 0);
   RegisterVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool BatteryMonitor::Iterate()
{
  AppCastingMOOSApp::Iterate();

  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool BatteryMonitor::OnStartUp()
{
	AppCastingMOOSApp::OnStartUp();
  list<string> sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(m_MissionReader.GetConfiguration(GetAppName(), sParams)) {
    list<string>::iterator p;
    for(p=sParams.begin(); p!=sParams.end(); p++) {
      string line  = *p;
      string param = tolower(biteStringX(line, '='));
      string value = line;
      
      if(param == "foo") {
        //handled
      }
      else if(param == "bar") {
        //handled
      }
    }
  }
  
  RegisterVariables();
  return(true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void BatteryMonitor::RegisterVariables()
{
  // Register("FOOBAR", 0);
  AppCastingMOOSApp::RegisterVariables();
  Register("BATTERY_NMEA_STRING");
}

bool BatteryMonitor::buildReport()
{
  m_msgs << "BATT_CURRENT: " << m_current << endl; 
  m_msgs << "BATT_VOLTAGE: " << m_voltage << endl; 
  m_msgs << "BATT_mAH: " << m_mAh << endl; 
  m_msgs << "BATT_%_REMAINING: " << m_percent_remaining << endl; 
  m_msgs << "BATT_STATUS: " << m_status << endl; 
  return(true);
}

