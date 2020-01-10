#!/usr/bin/sudo /bin/bash

kill -TERM $(cat /var/run/dhcpd.pid)
kill -TERM $(ps aux | grep 'hostapd'| awk '{print $2}')
