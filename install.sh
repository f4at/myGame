#!/bin/bash

cd libraries

#
#echo "installing cmake"
#cd cmake-3.15.2
#./bootstrap
#make
#sudo make install
#cd ..
#

echo "installing libwebsockets"
cd libwebsockets
mkdir build
cd build
cmake ..
make
sudo make install

