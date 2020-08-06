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
 echo "${1}  ${2} ${3}"

if [ "${1}" == "html" ] ; then
    gedit "${2}" &
exit 0
fi 

if [ "${1}" == "DrumFile" ] ; then
echo "${1}  ${2}"
exit 0
fi 

if [ "${1}" == "mp4" ] ; then
echo "${1}  ${2}"
    smplayer  "${2}" &
exit 0
fi 

if [ "${1}" == "mid" ] ; then
echo "${1}  ${2}"
    muse  "${2}" &
exit 0
fi 
 

if [ "${1}" == "med" ] ; then
echo "${1}  ${2}"
    muse  "${2}" &
exit 0
fi 

if [ "${1}" == "mscz" ] ; then
echo "${1}  ${2}"
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
echo "${1}  ${2}"
    okular  "${2}" --page="${3}"  &
exit 0
fi 


if [ "${1}" == "guitarix" ] ; then
    #       jackd -R -t5000 -dalsa -Chw:$AudioInHW$DeviceAdder -Phw:$AudioOutHW$DeviceAdder -r$AudioSampleRate -p128 -n3 &
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
#    /usr/bin/guitarix \
    -p 7000 &

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
