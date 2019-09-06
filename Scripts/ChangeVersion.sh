
theVersion="1.3.3"
sed '/^AC_INIT/s/.*/AC_INIT([LiveMusicApp], $theVersion],/' configure.ac 
sed '/^Standards-Version/s/.*/Standards-Version: $theVersion/' debian/control
