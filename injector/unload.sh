#!/usr/bin/sudo /bin/bash

kill -TERM $(cat /var/run/dhcpd.pid)
kill -TERM $(ps aux | grep 'hostapd'| awk '{print $2}')

ip addr del 10.5.19.1/24 dev wlp1s0 &1>/dev/null &2>/dev/null

