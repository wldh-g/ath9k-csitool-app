# *read_log* Data Instruction

In *read_log*, each packet cell consists of:

`timestamp`, `csi_len`, `channel`, `phy_err`, `noise`, `rate`, `bandwidth`, `nc`, `nr`, `nt`, `rssi`, `rssi1`, `rssi2`, `rssi3`, `payload_len`, `csi`

##### timestamp

A timestamp recorded by receiver hardware. Unit is microseconds.

##### csi_len - TODO

The length of CSI data in bytes. `= 2.5 * nt * nr * nc`

##### channel

The center frequency of the channel in MHz.

##### phy_err

PHY layer error code from ath9k driver. Look [here](https://github.com/wldh-g/BPI-R2-Atheros-CSITool/blob/5.4-main/drivers/net/wireless/ath/ath9k/mac.h#L193-L230) for detailed code information.

##### noise - TODO

##### rate

[HT MCS](http://mcsindex.com/). If `csi_len` is 0, this can mean CCK or OFDM rate.

If `logcsi` in this repository was used to make log, it automatically removes HT flag (7th bit) to reveal the MCS number more clearly.

##### bandwidth

+ `0` : 20MHz
+ `1` : 40MHz

##### nc, nr, nt, nt_actual

+ `nc` : The number of subcarriers
+ `nr` : The number of receive antennas
+ `nt` : The number of transmit antennas
+ `nt_actual` :

##### rssi, rssi1, rssi2, rssi3 - TODO

##### payload_len - TODO

The length of payload in bytes. `= injection packet size + 26`

##### csi - TODO
