
# autoscan
# mv configure.scan configure.ac
# libtoolize
aclocal 
autoheader
automake --add-missing 
autoconf
./configure 
