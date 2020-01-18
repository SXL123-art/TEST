#!/bin/sh

pkill -9 cas-app
pkill -9 disp
pkill -9 updateclient

rm -rf /lib/libAT*
rm -rf /root/Application/CASDATA_NEW.db
rm -rf /root/Application/ct100gc.sh
rm -rf /root/Application/disp
rm -rf /root/Application/cas-app
rm -rf /root/Application/updateclient
rm -rf /root/Application/scale_700.txt
rm -rf /root/Application/scale_430.txt
rm -rf /root/Application/share.txt
rm -rf /root/Application/script.txt

pkill -9 CT100App
pkill -9 CT100Qt
pkill -9 CT100Man

appdir='/root/Application/update/Application/'
libdir='/root/Application/update/lib/'
ftpdir='/root/Application/update/vsftpd/'
wifidir='/root/Application/update/wifi_8188/'

if [ -d "$appdir" ];then
cp -rf $appdir/* /root/Application/
fi

if [ -d "$libdir" ];then
cp -rf $libdir/* /lib/
fi

if [ -d "$ftpdir" ];then
exec $ftpdir/copyFile.sh
fi

if [ -d "$wifidir" ];then
cp -rf $wifidir /
fi

cd /root/Application/

rm -rf /root/Application/update
rm -rf /root/Application/update.tar.bz2
sync
sync
reboot
reboot


