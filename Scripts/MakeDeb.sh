#!/bin/bash

rm -rf debian/livemusicapp *.deb *.buildinfo *.changes *.ddeb debian/.debhelper debian/debhelper-build-stamp
rm ./debian/livemusicapp/ -rf

echo "Edit changelog for the correct version"

dpkg-buildpackage -b -uc -us
