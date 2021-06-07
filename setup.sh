#!/bin/bash
cd ~
if [ -d Robocup2dInstall-master ];then
	rm -r Robocup2dInstall-master
fi
if [ -f master ];then
	rm master
fi

wget  https://codeload.github.com/drdeng/Robocup2dInstall/zip/master
sudo apt-get install -y unzip
unzip  master
cd  Robocup2dInstall-master
chmod +x install.sh
./install.sh
