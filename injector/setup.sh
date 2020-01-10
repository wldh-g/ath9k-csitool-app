#!/usr/bin/sudo /bin/bash

ip addr add 10.5.19.1/24 dev wlp1s0 &1>/dev/null &2>/dev/null
systemctl stop isc-dhcp-server &1>/dev/null &2>/dev/null
dhcpd -cf ./dhcpd.conf
hostapd -d ./hostapd.conf &

echo "Configured."
