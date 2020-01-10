#!/bin/bash

ip link set wlp1s0 down
ip link set wlp1s0 address 12:34:56:78:90:ff
ip link set wlp1s0 up

iw wlp1s0 connect Atheros-CSITool

while [ $(iw wlp1s0 info | grep ssid | awk '{print $2}') != "Atheros-CSITool" ]; do
	iw wlp1s0 connect Atheros-CSITool
done

echo "Configured."
