#--------------------------------------------------------
# The CMakeLists.txt for:                       pPrimeFactor
# Author(s):                              Isaac Perper
#--------------------------------------------------------

SET(SRC
  PrimeFactor.cpp
  PrimeFactor_Info.cpp
  Number.cpp
  main.cpp
)

ADD_EXECUTABLE(pPrimeFactor ${SRC})

TARGET_LINK_LIBRARIES(pPrimeFactor
   ${MOOS_LIBRARIES}
   mbutil
   m
   pthread)

