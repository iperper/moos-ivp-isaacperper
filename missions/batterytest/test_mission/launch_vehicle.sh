# STARTUP FLAG
#    "sim"        Launch simulated vehicle and shoreside on this machine
#    "simNoshore" Launch only simulated vehicle on this machine
#    "shore"      Launch only shore on this machine
#    "real"       Launch only actual vehicle on this machine
JUST_SHORE="no"
LAUNCH_SHORE="no"
REAL_WINCH1="no"
REAL_VEHICLE1="no"
REAL_CTD1="no"
REAL_ALTIMETER1="no"
BAD_FLAGS="no"
PING_TEST="no"


# WARP          Sets the time warp
WARP1=1

# VNAME         Vehicle / community name
VNAME1="rex4USV"

# VPORT         MOOSDb IP port
VPORT1="9001"

# SHARE_LISTEN  IP port on vehicle for listening to incoming messages
SHARE_LISTEN1="9101"

# MULTICAST     Multicast_# the mission is using
MULTICAST1="multicast_10"

# HOME_POS      Location given as [xxx],[yyy] defining return point (and sim start location)
HOME_POS1="0,0"

# DRIFT_X, _Y   x and y components of simulated drift
ROTATE_SPEED1="0.0"
DRIFT_X1="0.0"
DRIFT_Y1="0.0"

# TOP_SPEED     Max vehicle speed in m/s
TOP_SPEED1="3.0"

# TRANSIT_SPEED Speed to use when transiting to next waypoint
TRANSIT_SPEED1=2.0

# RETURN_SPEED  Vehicle speed when returning home
RETURN_SPEED1=$TOP_SPEED1 

JUST_BUILD="no"

# Index of cast point that we start at, defaults to 0. All points before this point will be marked complete at init
INITAL_CAST_POINT_INDEX=0

#!/bin/bash 
#-------------------------------------------------------
#  Part 1: Check for and handle command-line arguments
#-------------------------------------------------------
JUST_MAKE="no"
for ARGI; do
    if [ "${ARGI}" = "--help" -o "${ARGI}" = "-h" ] ; then
        # display help
	    BAD_FLAGS="yes"
    elif [ "${ARGI//[^0-9]/}" = "$ARGI" -a "$WARP1" = 1 ]; then
        WARP1=$ARGI
    elif [ "${ARGI}" = "--just_build" -o "${ARGI}" = "-j" ] ; then
        JUST_BUILD="yes"
    elif [ "${ARGI}" = "--onlyShore" -o "${ARGI}" = "-o" ] ; then
        JUST_SHORE="yes"
    elif [ "${ARGI}" = "--shore" -o "${ARGI}" = "-s" ] ; then
        LAUNCH_SHORE="yes"
    elif [ "${ARGI}" = "--winchHW" -o "${ARGI}" = "-w" ] ; then
        REAL_WINCH1="yes"
    elif [ "${ARGI}" = "--ctdHW" -o "${ARGI}" = "-c" ] ; then
        REAL_CTD1="yes" 
    elif [ "${ARGI}" = "--vehicleHW" -o "${ARGI}" = "-v" ] ; then
        REAL_VEHICLE1="yes" 
    elif [ "${ARGI}" = "--altimeterHW" -o "${ARGI}" = "-a" ] ; then                                  
        REAL_ALTIMETER1="yes" 
    elif [ "${ARGI}" = "--pingTest" -o "${ARGI}" = "-p" ] ; then
        PING_TEST="yes"
    elif [[ ${ARGI} =~ ^-i=[0-9]+ ]] || [[ ${ARGI} =~ ^--initCast=[0-9]+ ]]; then
        INITAL_CAST_POINT_INDEX=$(echo ${ARGI} | grep -oe "[0-9]\{1,\}")
    else 
        BAD_FLAGS="yes"
	    printf "Bad Argument: %s \n" $ARGI
    fi
done

echo $INITAL_CAST_POINT_INDEX

if [ "${BAD_FLAGS}" = "yes" ] ; then
        printf "%s [SWITCHES] [time_warp]\n" $0
        printf "  --onlyShore,   -o    Run only the shoreside, no vehicle community\n"
        printf "  --shore,       -s    Run the shoreside (if omitted, no shoreside community is launched)\n"
        printf "  --winchHW,     -w    Use actual winch hardware (if omitted, runs uRex4UI)\n"
        printf "  --ctdHW,       -c    Use actual CTD hardware (if omitted, runs uTimerScript)\n"
        printf "  --vehicleHW,   -v    Use actual vehicle motors (if omitted, runs iActiationWAMV and iGPSDevice)\n"
        printf "  --altimeterHW, -a    Use actual altimeter (if omitted, does not run anything instead)\n"
        printf "  --initCast=x,  -i=x  Set the initial cast point to index x\n"
        printf "  --just_make,   -j    Just make files indicated by switches above\n"
        printf "  --help,        -h    Display this usage list\n" 
        exit 0;
    exit 0
fi

#-------------------------------------------------------
#  Part 2: Create the .moos and .bhv files. 
#-------------------------------------------------------
printf "****************** REAL_VEHICLE   = ${REAL_VEHICLE1}\n"
printf "****************** REAL_WINCH     = ${REAL_WINCH1}\n"
printf "****************** REAL_CTD       = ${REAL_CTD1}\n"
printf "****************** REAL_ALTIMETER = ${REAL_ALTIMETER1}\n"
printf "****************** TIME_WARP      = ${WARP1}\n"
printf "****************** INITAL_CAST_POINT_INDEX = ${INITAL_CAST_POINT_INDEX}\n"

printf "Preparing vehicle .moos file...\n"
# Conditionally Prepare Athena files
    nsplug meta_vehicle.moos targ_$VNAME1.moos -f         \
        REAL_VEHICLE=$REAL_VEHICLE1                       \
        REAL_CTD=$REAL_CTD1                               \
        REAL_WINCH=$REAL_WINCH1                           \
        REAL_ALTIMETER=$REAL_ALTIMETER1                   \
        WARP=$WARP1                                       \
        VNAME=$VNAME1                                     \
	      VPORT=$VPORT1                               \
	      SHARE_LISTEN=$SHARE_LISTEN1                 \
        MULTICAST=$MULTICAST1                             \
        HOME_POS=$HOME_POS1                               \
        ROTATE_SPEED=$ROTATE_SPEED1                       \
        DRIFT_X=$DRIFT_X1                                 \
        DRIFT_Y=$DRIFT_Y1                                 \
        TOP_SPEED=$TOP_SPEED1                             \
        INITAL_CAST_POINT_INDEX=$INITAL_CAST_POINT_INDEX

printf "Preparing vehicle .bhv file...\n"
    nsplug meta_vehicle.bhv targ_$VNAME1.bhv -f           \
        REAL_VEHICLE=$REAL_VEHICLE1                       \
        VNAME=$VNAME1                                     \
        HOME_POS=$HOME_POS1                               \
        RETURN_PT=$RETURN_PT1                             \
        TOP_SPEED=$TOP_SPEED1                             \
        RETURN_SPEED=$RETURN_SPEED1                       \
        TRANSIT_SPEED=$TRANSIT_SPEED1                     
    

if [ ${JUST_BUILD} = "yes" ] ; then
    exit 0
fi

#-------------------------------------------------------
#  Part 3: Launch the processes
#-------------------------------------------------------

if [ ${JUST_SHORE} = "yes" ] ; then
    ./launch_shoreside.sh -m $MULTICAST1 $WARP1
fi

if [ ${LAUNCH_SHORE} = "yes" ] ; then
    ./launch_shoreside.sh -m $MULTICAST1 $WARP1 >& /dev/null &
fi

if [ ${JUST_SHORE} = "no" ] ; then
    printf "Launching $VNAME1 MOOS Community \n"
    pAntler targ_$VNAME1.moos >& /dev/null &
    if [ ${PING_TEST} = "no" ] ; then
        uMAC targ_$VNAME1.moos
    else
        ping 192.168.1.2 | uPingTest targ_$VNAME1.moos
    fi
fi

if [ "${ANSWER}" = "2" ]; then
    printf "Killing all processes ... \n "
    kill %1 
    printf "Done killing processes.   \n "
fi


