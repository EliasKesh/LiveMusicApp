#! /bin/sh

if [ "$1" == "clean" ]; then
rm -rf LiveMusicApp config.log \
	Makefile config.status configure \
	aclocal.m4 m4 compile depcomp \
	missing config.guess config.sub \
	src/makefile stamp-h1 ltmain.sh \
	debian/livemusicapp debian/.debhelper
rm ChartFixer/makefile
rm .deps ./src/.deps ./ChartFixer/.deps -rf
rm makefile autom4te.cache -rf
else
# autoscan
# mv configure.scan configure.ac
libtoolize
aclocal 
autoheader
automake --add-missing 
autoconf
./configure
make
fi 

