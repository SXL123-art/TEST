#!/bin/sh
pkill -9 CT100App
pkill -9 CT100Qt
pkill -9 CT100Man

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

appdir='/root/Application/update/Application/'
libdir='/root/Application/update/lib/'
ftpdir='/root/Application/update/vsftpd/'
wifidir='/root/Application/update/wifi_8188/'

if [ -d "$appdir" ];then
cp -rf $appdir/* /root/Application/
echo $appdir
fi

if [ -d "$libdir" ];then
cp -rf $libdir/* /lib/
echo $libdir
fi

if [ -d "$ftpdir" ];then
cp -rf /root/Application/update/vsftpd/etc/* /etc/
cp -rf /root/Application/update/vsftpd/usr/* /usr/
cp -rf /root/Application/update/vsftpd/var/* /var/
echo $ftpdir
fi

if [ -d "$wifidir" ];then
cp -rf $wifidir/* /wifi_8188/
echo $wifidir
fi

rm -rf /root/Application/update
#rm -rf /root/Application/update.tar.bz2
sync
sync
reboot
reboot


