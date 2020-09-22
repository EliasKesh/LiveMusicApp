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
echo "MusicApp -> ${1}  ${2} ${3}"

if [ "${1}" == "html" ] ; then
    gedit "${2}" &
exit 0
fi 

if [ "${1}" == "DrumFile" ] ; then
exit 0
fi 

# if [ "${1}" == "mp4" ] ; then
#     smplayer  "${2}" &
# exit 0
# fi 

if [ "${1}" == "mid" ] ; then
    muse  "${2}" &
exit 0
fi 

if [ "${1}" == "med" ] ; then
    muse  "${2}" &
exit 0
fi 

if [ "${1}" == "mscz" ] ; then
    musescore  "${2}" &>/dev/null &
exit 0
fi 

if [ "${1}" == "gp3" ] | 
   [ "${1}" == "gp4" ] |  
   [ "${1}" == "gp5" ] |  
   [ "${1}" == "gp6" ] |  
   [ "${1}" == "ptb" ] ; then
echo "${1}  ${2}"
    musescore  "${2}"  &>/dev/null &
exit 0
fi 

if [ "${1}" == "pdf" ] ; then
    okular  "${2}" --page="${3}"  &
exit 0
fi 

if [ "${1}" == "guitarix" ] ; then
    #jackd -R -t5000 -dalsa -Chw:$AudioInHW$DeviceAdder -Phw:$AudioOutHW$DeviceAdder -r$AudioSampleRate -p128 -n3 &
    echo "Running guitarix"
    GDK_BACKEND=x11 nice -15 \
    /AppImages/guitarix-0.39-x86_64.AppImage \
    -p 7000 &
exit 0
fi

if [ "${1}" == "EffectsProcessorApp" ] ; then
    #       jackd -R -t5000 -dalsa -Chw:$AudioInHW$DeviceAdder -Phw:$AudioOutHW$DeviceAdder -r$AudioSampleRate -p128 -n3 &
    echo "Running guitarix"
    GDK_BACKEND=x11 nice -15 \
    /AppImages/guitarix-0.39-x86_64.AppImage \
    -p 7000 &

#    /usr/bin/guitarix \
#    /AppImages/guitarix-0.39-x86_64.AppImage \
exit 0
fi

if [ "${1}" == "guitarixNew" ] ; then
    #       jackd -R -t5000 -dalsa -Chw:$AudioInHW$DeviceAdder -Phw:$AudioOutHW$DeviceAdder -r$AudioSampleRate -p128 -n3 &
    echo "Running guitarix"
    GDK_BACKEND=x11 nice -15 \
        guitarix \
        -p 7000 &
exit 0
fi

exit 1
