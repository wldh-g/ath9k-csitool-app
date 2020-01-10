# BPI-R2 Atheros CSITool Apps

This repository contains modified csitool-apps for BPI-R2.

This tool only supports ath9k-based CSI collection and you must install [modified kernel](https://github.com/wldh-g/BPI-R2-Atheros-CSITool) before run these apps.

### How To Record CSI

##### Transmitter

Before contiune, install `hostapd`, `isc-dhcp-server`.

```sh
git clone https://github.com/wldh-g/BPI-R2-Atheros-CSITool-App.git
cd BPI-R2-Atheros-CSITool-App/injector
./setup.sh
make
./inject help
```

You can configure MCS, channel at  `injector/hostapd.conf`.

##### Receiver

```sh
git clone https://github.com/wldh-g/BPI-R2-Atheros-CSITool-App.git
cd BPI-R2-Atheros-CSITool-App/injector
./setup.sh
make
./recv_csi [file name]
```

### How To Use Recorded CSI

1. Compile `reader/read_csi.c`.
2. Parse recorded data using `read_log_file.m`.

For more information, look at [the project homepage](https://wands.sg/research/wifi/AtherosCSI/).
