# Atheros CSITool Apps

This repository contains modified csitool-apps.

This tool only supports ath9k-based CSI collection and you must install the modified kernel for csitool to work before run these apps.

These apps are tested on BPI-R2 device with this [modified kernel for BPI-R2](https://github.com/wldh-g/ath9k-csitool-r2).

### How To Record Packets

##### Transmitter

Before contiune, install `libnl-3`, `libnl-3-dev`, `isc-dhcp-server`.

```sh
git clone https://github.com/wldh-g/ath9k-csitool-apps.git --recurse-submodules
cd ath9k-csitool-apps/injector
make -j4 # Build injector and hostapd
./load.sh -h # Prints help message
sudo ./inject -h # Prints help message
```

You can configure channels and HT mode using `./load.sh`. Run `unload.sh` before rerun `load.sh` to change the configuration.

To turn off dhcp server and hostapd, unload the injector:

```sh
./unload.sh
```

##### Receiver

```sh
git clone https://github.com/wldh-g/ath9k-csitool-apps.git
cd ath9k-csitool-apps/receiver
make # Build logger
./connect.sh # Do this after each load at transmitter
sudo ./logcsi -h
```

The MAC address of `wlp1s0` will be changed to `12:34:56:78:90:ff` after execute `setup.sh`.

### How To Use Recorded Logs

Parse recorded data using `log_reader/read_log.m` in MATLAB.

For more information, look at [the project homepage](https://wands.sg/research/wifi/AtherosCSI/).
