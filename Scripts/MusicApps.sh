#! /bin/sh
#---------------------------------------------------------------------
#
#	File: 	LiveMusicApps
#
#	Contains: 	
#
#
#	Written By: 	Elias Keshishoglou on Wed 17 Jun 2020 04:52:30 PM PDT
#
#	Copyright ©: 	2020 Elias Keshishoglou all rights reserved.
#
#
#---------------------------------------------------------------------#

if [ "${1}" == "guitarix" ] ; then
    #       jackd -R -t5000 -dalsa -Chw:$AudioInHW$DeviceAdder -Phw:$AudioOutHW$DeviceAdder -r$AudioSampleRate -p128 -n3 &
    echo "Running guitarix"
    GDK_BACKEND=x11 nice -15 \
    /AppImages/guitarix-0.39-x86_64.AppImage \
    -p 7000 &
fi

if [ "${1}" == "EffectsProcessorApp" ] ; then
    #       jackd -R -t5000 -dalsa -Chw:$AudioInHW$DeviceAdder -Phw:$AudioOutHW$DeviceAdder -r$AudioSampleRate -p128 -n3 &
    echo "Running guitarix"
    GDK_BACKEND=x11 nice -15 \
#    /AppImages/guitarix-0.39-x86_64.AppImage \
    /usr/bin/guitarix \
    -p 7000 &

#    /AppImages/guitarix-0.39-x86_64.AppImage \

fi

if [ "${1}" == "guitarixNew" ] ; then
    #       jackd -R -t5000 -dalsa -Chw:$AudioInHW$DeviceAdder -Phw:$AudioOutHW$DeviceAdder -r$AudioSampleRate -p128 -n3 &
    echo "Running guitarix"
    GDK_BACKEND=x11 nice -15 \
        guitarix \
        -p 7000 &
fi
