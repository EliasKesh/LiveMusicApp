

NewVersion="1.9.0"
sed -i "/Version = /s/.*/    Version = \"$NewVersion\"/" Scripts/LiveMusicCharts.py
sed -i "/echo /s/.*/echo \"$NewVersion\"/" Scripts/version.sh
sed -i "/^AC_INIT/s/.*/AC_INIT([LiveMusicApp], \[$NewVersion\],/" configure.ac 
sed -i "/^Standards-Version/s/.*/Standards-Version: $NewVersion/" debian/control
sed -i "/^Version/s/.*/Version: $NewVersion/" debian/control
echo "\""$NewVersion"\"" > ./Scripts/version.txt
# Need to change changelog

# grep -r "1\.3\.4" *
