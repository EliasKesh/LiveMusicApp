

NewVersion="1.8.2"
sed -i "/Version=/s/.*/  Version=$NewVersion/" Scripts/LiveMusicCharts.py
sed -i "/^AC_INIT/s/.*/AC_INIT([LiveMusicApp], \[$NewVersion\],/" configure.ac 
sed -i "/^Standards-Version/s/.*/Standards-Version: $NewVersion/" debian/control
sed -i "/^Version/s/.*/Version: $NewVersion/" debian/control

# Need to change changelog

# grep -r "1\.3\.4" *
