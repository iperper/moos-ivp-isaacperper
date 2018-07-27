#--------------------------------------------------------
# The CMakeLists.txt for:                       pPointAssign
# Author(s):                              Isaac Perper
#--------------------------------------------------------

SET(SRC
  PointAssign.cpp
  Point.cpp
  PointAssign_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pPointAssign ${SRC})

TARGET_LINK_LIBRARIES(pPointAssign
   ${MOOS_LIBRARIES}
   apputil
   geometry
   mbutil
   m
   pthread
   )

