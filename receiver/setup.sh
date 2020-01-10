#!/bin/bash

ip link set wlp1s0 down
ip link set wlp1s0 address 12:34:56:78:90:ff
ip link set wlp1s0 up

iw wlp1s0 connect Atheros-CSITool

echo "Configured."
