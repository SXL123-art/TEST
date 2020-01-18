insmod wlan.ko

./wpa_supplicant -Dwext -iwlan0 -C/wifi_8188/configure/wpa_supplicant -c/wifi_8188/configure/wpa_0_8.conf &  
./hostapd rtl_hostapd.conf -B
./wpa_cli -p/wifi_8188/configure/wpa_supplicant remove_network 0	#移除先前网络，如果先前没有返回失败。不影响后面的操作
./wpa_cli -p/wifi_8188/configure/wpa_supplicant ap_scan 1
./wpa_cli -p/wifi_8188/configure/wpa_supplicant add_network
./wpa_cli -p/wifi_8188/configure/wpa_supplicant set_network 0 ssid '"TPLINK_CAS"'	#换成你们的njrc0001
./wpa_cli -p/wifi_8188/configure/wpa_supplicant set_network 0 psk '"libaoli371039"'	#也就是密码是12345678
./wpa_cli -p/wifi_8188/configure/wpa_supplicant select_network 0

#ifconfig eth0 down
#ifconfig wlan0 up
#ifconfig wlan0 192.168.1.103 netmask 255.255.255.0
#route add default gw 192.168.1.1
