# *logcsi* Data Structure

In *logcsi*, meanings of the variables are like below:

+ `read_size` = A + B + C + D
+ `buf_len` = A + B + C
+ `csi_len` = B
+ `payload_len` = C

#### A. CSI Status `25 bytes`

 1. `8 bytes [0:7]`   - Timestamp
 2. `2 bytes [8:9]`   - CSI Data Length
 3. `2 bytes [10:11]` - Channel Number
 4. `1 byte [12]`     - PHY Error Information
 5. `1 byte [13]`     - Floor Noise
 6. `1 byte [14]`     - Tx Rate
 7. `1 byte [15]`     - Bandwidth
 8. `1 byte [16]`     - The Number Of Subcarriers
 9. `1 byte [17]`     - The Number Of Receiver Antennas
10. `1 byte [18]`     - The Number Of Transmitter Antennas
11. `1 byte [19]`     - Rx Frame RSSI
12. `1 byte [20]`     - Rx Frame RSSI [ctl, chain 0]
13. `1 byte [21]`     - Rx Frame RSSI [ctl, chain 1]
14. `1 byte [22]`     - Rx Frame RSSI [ctl, chain 2]
15. `2 bytes [23:24]` - Payload Length

#### B. CSI Data

`140 bytes` for each Tx/Rx pair.

#### C. Payload

The size of payload is `(injection packet size + 26) bytes`.

#### D. Buffer Information `2 bytes`

1. `2 bytes` - Buffer Length
