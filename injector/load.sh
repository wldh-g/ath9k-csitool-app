#!/usr/bin/sudo /bin/bash

ip addr add 10.5.19.1/24 dev wlp1s0 > /dev/null 2>&1
systemctl stop isc-dhcp-server > /dev/null 2>&1
dhcpd -cf ./dhcpd.conf
hostapd -d ./hostapd.conf -B

echo "Injector Configured."
