#!/usr/bin/sudo /bin/bash

print_help() {
	echo "Usage   : ./load.sh CHANNEL_INDEX [+|-]"
	echo "Example : ./load.sh 1 +"
	echo "Example : ./load.sh 6"
	exit
}

if [ "$1" == "--help" ]; then
	print_help
elif [ "$1" == "-h" ]; then
	print_help
elif [ "$1" == "help" ]; then
	print_help
elif [ "$1" == "" ]; then
	echo "Set channel, please!"
	print_help
else
	cp ./hostapd.default.conf ./hostapd.temp.conf
	echo "channel="$1 >> ./hostapd.temp.conf
fi

if [ "$2" == "" ]; then
	echo "ht_capab=[SHORT-GI-20][HT20]" >> ./hostapd.temp.conf
elif [ "$2" == "+" ]; then
	echo "ht_capab=[SHORT-GI-40][HT40+]" >> ./hostapd.temp.conf
elif [ "$2" == "-" ]; then
	echo "ht_capab=[SHORT-GI-40][HT40-]" >> ./hostapd.temp.conf
else
	echo "Incorrect HT mode character!"
	rm ./hostapd.temp.conf
	print_help
fi

ip addr add 10.5.19.1/24 dev wlp1s0 > /dev/null 2>&1
systemctl stop isc-dhcp-server > /dev/null 2>&1
dhcpd -cf ./dhcpd.conf
./hostap/hostapd/hostapd -d ./hostapd.temp.conf -B

rm ./hostapd.temp.conf

echo "Injector Configured."
