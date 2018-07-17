#!/bin/bash 
#-------------------------------------------------------
#  Part 1: Check for and handle command-line arguments
#-------------------------------------------------------

TIME_WARP=1
JUST_BUILD="no"
SIMULATION="no"
MULTICAST_SHORE="none"
NEXT_IS_MULTI="no"
NAME_SHORE="shoreside"
PORT_SHORE="9000"

for ARGI; do
    if [ "${ARGI}" = "--help" -o "${ARGI}" = "-h" ] ; then
	printf "%s [SWITCHES] [time_warp]   \n" $0
	printf "  --just_make, -j    \n" 
	printf "  --help, -h         \n" 
	exit 0;
    elif [ "${ARGI//[^0-9]/}" = "$ARGI" -a "$TIME_WARP" = 1 ]; then 
        TIME_WARP=$ARGI
    elif [ "${ARGI}" = "--just_build" -o "${ARGI}" = "-j" ] ; then
	JUST_BUILD="yes"
    elif [ "${ARGI}" = "--simulate" -o "${ARGI}" = "-s" ] ; then
        SIMULATION="yes"
    elif [ "${ARGI}" = "--multicast" -o "${ARGI}" = "-m" ] ; then
        NEXT_IS_MULTI="yes"
    elif [ "${NEXT_IS_MULTI}" = "yes" ] ; then
        MULTICAST_SHORE=$ARGI
        NEXT_IS_MULTI="no"
    else 
	printf "Bad Argument: %s \n" $ARGI
	exit 0
    fi
done

#-------------------------------------------------------
#  Part 2: Create the .moos and .bhv files. 
#-------------------------------------------------------

printf "Preparing shoreside .moos file...\n"
nsplug meta_shoreside.moos targ_shoreside.moos -f WARP=$TIME_WARP \
       SNAME=$NAME_SHORE                                          \
       SPORT=$PORT_SHORE                                          \
       MULTICAST=${MULTICAST_SHORE}

if [ ${JUST_BUILD} = "yes" ] ; then
    exit 0
fi

#-------------------------------------------------------
#  Part 3: Launch the processes
#-------------------------------------------------------
printf "Launching shoreside MOOS Community (WARP=%s) \n"  $TIME_WARP
pAntler targ_shoreside.moos >& /dev/null &
printf "Done \n"

if [ ${SIMULATION} = "yes" ] ; then
    exit 0
fi

uMAC targ_shoreside.moos

printf "Killing all processes ... \n"
kill %1 
printf "Done killing processes.   \n"


