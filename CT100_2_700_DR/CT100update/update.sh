#!/bin/sh

pkill -9 CT100App
pkill -9 CT100Qt
pkill -9 CT100Man

cp -rf /root/Application/CT100update/Application/* /root/Application/
cp -rf /root/Application/CT100update/lib/* /lib/

cd /root/Application/
rm -rf /root/Application/CT100update/
rm -f /root/Application/CT100update.tar.gz

sync
sync

reboot

