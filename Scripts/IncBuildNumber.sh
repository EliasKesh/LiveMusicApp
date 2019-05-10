oldnum=`cat ./BuildNumber`  
newnum=`expr $oldnum + 1`
echo $newnum > BuildNumber  
echo $newnum 