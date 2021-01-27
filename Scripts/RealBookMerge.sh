#! /bin/sh
#------------------------------------------------
#
#	File: 	RealBookMerge.sh
#
#	Contains: 	
#
#
#	Written By: 	Elias Keshishoglou on Tue 09 Jul 2019 07:14:49 AM PDT
#
#	Copyright ©: 	2019 Elias Keshishoglou all rights reserved.
#
#
#------------------------------------------------#
rm temp.csv temp1.csv TotalData1.csv TotalData2.csv TotalData.csv
for i in `ls -v *.csv` ; do

echo $i
# Copy to Temp.
cp $i temp.csv

# Remove Header
#sed -i 1d temp.csv

# Get the date.
fbname=$(basename "$i" .csv)
echo $fbname

# Add the date to the table.
# awk -F, '{$fbname;}1' OFS=, temp.csv
sed "s/^/$fbname.pdf,/" "temp.csv" > temp1.csv

cat temp1.csv >> TotalData.csv

done;
sort --field-separator=',' --key=2 TotalData.csv > TotalData1.csv

