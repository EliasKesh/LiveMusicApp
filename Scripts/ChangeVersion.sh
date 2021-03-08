
sed -i '/^AC_INIT/s/.*/AC_INIT([LiveMusicApp], \[1.8.1\],/' configure.ac 
sed -i '/^Standards-Version/s/.*/Standards-Version: 1.8.1/' debian/control
sed -i '/^Version/s/.*/Version: 1.8.1/' debian/control
# Need to change changelog

# grep -r "1\.3\.4" *
