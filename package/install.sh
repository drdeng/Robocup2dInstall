#!/bin/bash
tar -zxvf  rcssserver-16.0.0.tar.gz
cd rcssserver-16.0.0
./configure  
make
sudo make install
cd ..

tar -zxvf rcssmonitor-16.0.0.tar.gz 
cd rcssmonitor-16.0.0
./configure  
make
sudo make install 
cd ..



unzip librcsc-develop.zip 
cd librcsc-develop
./bootstrap
./configure 
make
sudo make install
cd ..


unzip soccerwindow2-develop.zip
cd soccerwindow2-develop
./bootstrap
./configure 
make
sudo  make install
cd ..

unzip fedit2-develop.zip
cd fedit2-develop
./bootstrap
./configure 
make
sudo make install
