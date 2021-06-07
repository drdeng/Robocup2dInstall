#!/bin/bash
DIR=$(dirname $0)
cd $DIR




sudo apt-get update
sudo apt-get install -y  bison libbison-dev
sudo apt install -y  qtbase5-dev



sudo apt-get install -y build-essential libboost-all-dev flex 
sudo apt-get install -y libxpm-dev libaudio-dev libxt-dev libpng-dev libglib2.0-dev libfreetype6-dev libxrender-dev libxext-dev libfontconfig-dev libxi-dev



cd  ./package
sudo sh  install.sh



#将lib添加到搜索

sudo echo "include /etc/ld.so.conf.d/*.conf
/usr/lib
/usr/local/lib
">ld.so.conf
sudo rm   /etc/ld.so.conf
sudo mv  ld.so.conf  /etc

sudo ldconfig


