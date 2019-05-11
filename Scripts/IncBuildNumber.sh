oldnum=`cat ./Scripts/BuildNumber`  
newnum=`expr $oldnum + 1`
echo $newnum > ./Scripts/BuildNumber  
echo $newnum 