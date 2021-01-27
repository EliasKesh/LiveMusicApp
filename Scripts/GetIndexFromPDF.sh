#! /bin/sh
#------------------------------------------------
#
#	File: 	GetIndexFromPDF
#
#	Contains: 	
#
#
#	Written By: 	Elias Keshishoglou on Wed 12 Feb 2020 08:18:48 AM PST
#
#	Copyright ©: 	2020 Elias Keshishoglou all rights reserved.
#
#
#------------------------------------------------#
# find ./ -iname \*pdf -exec "java  $JVM_OPTIONS -jar /usr/src/jpdfbookmarks-2.5.2/jpdfbookmarks.jar -d {}" > {}.txt1 \;
# 
for i in `ls -v *.pdf` ; do

echo $i
# Get the date.
fbname=$(basename "$i" .pdf)

NewName=$fbname".txt"
echo $fbname"  ", $NewName

java  $JVM_OPTIONS -jar /usr/src/jpdfbookmarks-2.5.2/jpdfbookmarks.jar -d $i > $NewName

# Add the date to the table.
# awk -F, '{$fbname;}1' OFS=, temp.csv
#sed "s/^/$fbname.pdf,/" "temp.csv" > temp1.csv

#cat temp1.csv >> TotalData.csv

done;
