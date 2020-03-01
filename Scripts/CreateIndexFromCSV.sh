#! /bin/sh
#---------------------------------------------------------------------
#
#	File: 	CreateIndexFromCSV
#
#	Contains: 	
#
#
#	Written By: 	Elias Keshishoglou on Tue 09 Jul 2019 03:17:08 PM PDT
#
#	Copyright ©: 	2019 Elias Keshishoglou all rights reserved.
#
#
#---------------------------------------------------------------------#

echo "<html><head><style type=\"text/css\">" > index.html
echo "{text-align: left; width: 100%; height: 100%;}" >> index.html
echo "body {background-image: url(./background.png);background-repeat: no-repeat;
" >> index.html
echo "background-size: 100%;color: orange;height: 100%; }" >> index.html
echo "</style></head><title>FakeIndex</title><p style=\"line-height:160%\"><body>" >> index.html
# \nbackground-color: #000000;

echo "<body link="#ffd0a0" vlink="#d08080" alink="#FF0000"><big><p id=\"#TOP\"></p><a href=\"#Aaaa\">[ __A__ ]</a><a href=\"#Baaa\">[ __B__ ]</a><a href=\"#Caaa\">[ __C__ ]</a><a href=\"#Daaa\">[ __D__ ]</a><a href=\"#Eaaa\">[ __E__ ]</a><a href=\"#Faaa\">[ __F__ ]</a><a href=\"#Gaaa\">[ __G__ ]</a><a href=\"#Haaa\">[ __H__ ]</a><a href=\"#Iaaa\">[ __I__ ]</a><a href=\"#Jaaa\">[ __J__ ]</a><a href=\"#Kaaa\">[ __K__ ]</a><a href=\"#Laaa\">[ __L__ ]</a><a href=\"#Maaa\">[ __M__ ]</a><a href=\"#Naaa\">[ __N__ ]</a><a href=\"#Oaaa\">[ __O__ ]</a><a href=\"#Paaa\">[ __P__ ]</a><a href=\"#Qaaa\">[ __Q__ ]</a><a href=\"#Raaa\">[ __R__ ]</a><a href=\"#Saaa\">[ __S__ ]</a><a href=\"#Taaa\">[ __T__ ]</a><a href=\"#Uaaa\">[ __U__ ]</a><a href=\"#Vaaa\">[ __V__ ]</a><a href=\"#Waaa\">[ __W__ ]</a><a href=\"#Xaaa\">[ __X__ ]</a><a href=\"#Yaaa\">[ __Y__ ]</a><a href=\"#Zaaa\">[ __Z__ ]</a><br>" >> index.html
echo "<code>" >> index.html

OldAIndex="z"

while read -r line
do
    Song=$(echo $line | awk -F',' '{printf "%s", $2}' | tr -d '"')
    Book=$(echo $line | awk -F',' '{printf "%s", $1}' | tr -d '"')
    Page=$(echo $line | awk -F',' '{printf "%s", $4}' | tr -d '"' | tr -d '\r' | tr -d ' ' )

    firstLetter=${Song:0:1}
    if  [[ "$firstLetter" =~ ^[A-Z]+$ ]]; then
        AIndex=$firstLetter    
    fi

    if [[ $AIndex != $OldAIndex ]]; then
        echo " **** New Index"
        echo "<p id=\""$AIndex"aaa\"></p><a style="color:red" href=\"#TOP\">TOP     </a><br>" >> index.html
    fi

    OldAIndex=$AIndex

    echo "Song ["$AIndex"] "$Song "Book "$Book" Page ["$Page"]"

    # Check for Page Number.
    re='^[0-9]+$'
    if ! [[ $Page =~ $re ]] ; then
        echo "error: Not a number" >&2; exit 1 
        WrapCount=0
    fi

    SongTitle=$Song"___________________________________________"
    SongLink=`echo $SongTitle | cut -c1-20`

    WrapCount=$((WrapCount+1))
    if [[ $WrapCount == 4 ]]; then
        echo "<br>" >> index.html
        WrapCount=0
    fi

    echo  "<a href=\"./$Book#page=$Page\">$SongLink </a> " >> index.html


done < $1
echo "</code>" >> index.html
echo "</tr></tt></code></big>\n</code>\n</p>\n</body>\n</html>" >> index.html
