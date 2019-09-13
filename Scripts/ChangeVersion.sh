
sed -i '/^AC_INIT/s/.*/AC_INIT([LiveMusicApp], \[1.3.5\],/' configure.ac 
sed -i '/^Standards-Version/s/.*/Standards-Version: 1.3.5/' debian/control
sed -i '/^Version/s/.*/Version: 1.3.5/' debian/control
# Need to change changelog

# grep -r "1\.3\.4" *
