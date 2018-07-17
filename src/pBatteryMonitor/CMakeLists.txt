#--------------------------------------------------------
# The CMakeLists.txt for:                       pBatteryMonitor
# Author(s):                              Isaac Perper
#--------------------------------------------------------

SET(SRC
  BatteryMonitor.cpp
  BatteryMonitor_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pBatteryMonitor ${SRC})

TARGET_LINK_LIBRARIES(pBatteryMonitor
   ${MOOS_LIBRARIES}
   mbutil
   m
   pthread)

