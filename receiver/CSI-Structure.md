# Detailed CSI Buffer Structure

In `logcsi`, meanings of the variables are like below:

+ `buf_len` = A + B + C
+ `payload_len` = C
+ `csi_len` = B
+ `read_size` = A + B + C + D

#### A. CSI Status `25 bytes`

 1. 8 bytes -- Timestamp
 2. 2 bytes -- CSI Data Length
 3. 2 bytes -- Channel Number
 4. 1 byte  -- PHY Error Information
 5. 1 byte  -- Floor Noise
 6. 1 byte  -- Tx Rate
 7. 1 byte  -- Bandwidth
 8. 1 byte  -- The Number Of Subcarriers
 9. 1 byte  -- The Number Of Receiver Antennas
10. 1 byte  -- The Number Of Transmitter Antennas
11. 1 byte  -- Rx Frame RSSI
12. 1 byte  -- Rx Frame RSSI [ctl, chain 0]
13. 1 byte  -- Rx Frame RSSI [ctl, chain 1]
14. 1 byte  -- Rx Frame RSSI [ctl, chain 2]
15. 2 byte  -- Payload Length

#### B. CSI Data

#### C. Payload

#### D. Buffer Information `2 bytes`

1. 2 bytes -- Buffer Length

