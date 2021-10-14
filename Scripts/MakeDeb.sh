rm ./debian/livemusicapp/ -rf

echo "Edit changelog for the correct version"

dpkg-buildpackage -b -uc -us
