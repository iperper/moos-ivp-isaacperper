/************************************************************/
/*    NAME: Isaac Perper                                              */
/*    ORGN: MIT                                             */
/*    FILE: BatteryMonitor.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef BatteryMonitor_HEADER
#define BatteryMonitor_HEADER

#include "MOOS/libMOOS/MOOSLib.h"
#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include <string>
#include <vector>

class BatteryMonitor : public AppCastingMOOSApp
{
 public:
   BatteryMonitor();
   ~BatteryMonitor();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();
   bool handleBattery(std::vector<std::string> &parsed);
   bool  buildReport();

 protected:
   void RegisterVariables();

 private: // Configuration variables

 private: // State variables
  double m_current, m_voltage, m_mAh, m_percent_remaining, m_min_SoC, m_min_voltage, m_batt_mAh;
  std::string  m_status;
};

#endif 
