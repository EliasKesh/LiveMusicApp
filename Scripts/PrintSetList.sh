#!/bin/bash
#--------------------------------------------------------------
#
#   File:   PrintSetList
#
#   Contains:
#
#
#   Written By:     Elias Keshishoglou on Sat Apr 8 07:19:51 PM PDT 2023
#
#   Copyright :     2023 Elias Keshishoglou all rights reserved.
#
#
#--------------------------------------------------------------#

SetListFile="${1}"

# The Charts file
TheOutputHTMLFile="${SetListFile}.Charts.html"

# The index and structure file

TheOutputIndHTMLFile="${SetListFile}.Index.html"
TitleName=`basename ${SetListFile}`
echo $TitleName

# Clear out the files before appending to them
echo "<html><big><big>" > "${TheOutputHTMLFile}"
#echo "<big>"`date`" <big><big> <center>$TitleName</center></big><br>" > "${TheOutputIndHTMLFile}"
echo "<html><big><big>" > "${TheOutputIndHTMLFile}"

# Get links from setlist
TheSetList=`lynx -listonly -nonumbers -dump  "${SetListFile}"`
echo "Set Lists "$(TheSetList)
# Walk thru all of the links in the SetList
for f in $TheSetList
do
    
    BaseName=`basename $f`
    RealPath1=`dirname $f`
    RealPath=`echo $RealPath1 | sed 's/^.\{7\}//g'`
    
    ChartName=$(basename $BaseName .html)
    echo "Set Lists "$(ChartName)
    # TheText=`w3m -dump $RealPath"/"$BaseName`
    # Get the text from the html files.
    TheText=`lynx -dump $RealPath"/"$BaseName`
    
    # Remove everything before the Structure:
    TheText1=`echo ${TheText} | sed 's/^.*Structure/Structure/'`
    # Remove everything after the References
    TheText2=`echo ${TheText1} | sed 's/References.*//'`
    
    # Print the Song name to the file
    echo "<big><big><big>${ChartName}</big></big></big><br>" >> "${TheOutputIndHTMLFile}"
    
    # Print the Structure to the file
    echo "<big>${TheText2}</big><br>" >> "${TheOutputIndHTMLFile}"
    
    # echo "Processing $RealPath / $BaseName"
    # Look for the printable files
    TheCharts=$(ls $RealPath/0P_*)
    # echo "Files $TheCharts"
    
    # Walk thru the charts in case there are multiple pages
    for c in $TheCharts
    do
        # echo "Charts $c"
        
        # Write the Chart image to the file.
        echo "<img alt=\"\" src=\"$c\" width=\"100%\" >" >> "${TheOutputHTMLFile}"
    done
    
    # Issue a page break.
    echo "<div class=\"pagebreak\"> </div>" >> "${TheOutputHTMLFile}"
    
done

# Open the browser to show the files.
vivaldi  "${TheOutputHTMLFile}" "${TheOutputIndHTMLFile}" &

# End here
exit

# Saved snippets
#TheOutputFile="${SetListFile}.out.pdf"

sed -n '/<LiveMusic>/,/<\/LiveMusic>/p' TeeTime/TeeTime.html

convert file1.pdf file2.pdf merged.pdf

w3m -dump StThomas.html
lynx  -dump StThomas.html
htmldoc --webpage -f output.pdf StThomas.html

pandoc input.txt -o output.pdf
# pandoc "${TheOutputTextFile}" -o ${TheOutputFile}
# always double quote "$f" filename
# do something on $f

#TheCharts1=`find $RealPath -iname 0P_* -exec echo {} \;`
#echo "Files1 $TheCharts1"
#TheCharts=`sort $TheCharts1 -nu`
#TheCharts=`(echo $TheCharts1 | sort -nu)`

#TheCharts=`"ls "$RealPath"/0P_*  "`


#    convert ${TheOutputFile} $c -resize 1240x1750 ${TheOutputFile}





