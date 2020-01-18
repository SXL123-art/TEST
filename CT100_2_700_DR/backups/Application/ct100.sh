pkill -9 CT100Man
pkill -9 CT100App
pkill -9 CT100Qt

cd /root/Application/modules
insmod ct100gc_printer.ko
insmod ct100gc_buzzer.ko
insmod ct100gc_ad.ko
insmod ct100gc_key.ko
insmod ct100gc_pmu.ko

cd /root/Application/
./CT100Man &

