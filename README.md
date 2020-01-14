# BPI-R2 Atheros CSITool Apps

This repository contains modified csitool-apps for BPI-R2.

This tool only supports ath9k-based CSI collection and you must install [modified kernel](https://github.com/wldh-g/BPI-R2-Atheros-CSITool) before run these apps.

### How To Record Packets

##### Transmitter

Before contiune, install `hostapd`, `isc-dhcp-server`.

```sh
git clone https://github.com/wldh-g/BPI-R2-Atheros-CSITool-App.git
cd BPI-R2-Atheros-CSITool-App/injector
make
./load.sh
sudo ./inject -h
```

You can configure MCS, channel at  `injector/hostapd.conf`.

To turn off dhcp server and hostapd, unload the injector:

```sh
./unload.sh
```

##### Receiver

```sh
git clone https://github.com/wldh-g/BPI-R2-Atheros-CSITool-App.git
cd BPI-R2-Atheros-CSITool-App/receiver
make
./setup.sh
sudo ./logcsi -h
```

The MAC address of `wlp1s0` will be changed to `12:34:56:78:90:ff` after execute `setup.sh`.

### How To Use Recorded Logs

Parse recorded data using `log_reader/read_log.m` in MATLAB.

For more information, look at [the project homepage](https://wands.sg/research/wifi/AtherosCSI/).
