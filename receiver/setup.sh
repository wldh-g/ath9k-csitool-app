#!/bin/bash

ip link set wlp1s0 down
echo "Your MAC address of wlp1s0 temporarily changed to 12:34:56:78:90:ff."
echo "The original MAC address was " $(cat /sys/class/net/wlp1s0/address) " ."
ip link set wlp1s0 address 12:34:56:78:90:ff
ip link set wlp1s0 up

iw wlp1s0 connect Atheros-CSITool

echo "Configured."
