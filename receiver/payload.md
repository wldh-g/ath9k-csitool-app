# Payload Example Analysis

For my understand. I will update this for publication later.

### `raw`

```python
>>> raw = b"\x88\x02\x3C\x00\x12\x34\x56\x78\x90\xFF\x28\xE3\x47\x42\x0A\x83\x28\xE3\x47\x42\x0A\x83\x10\x00\x06\x00\x10\xAA\xAA\xAA\x03\x00\x00\x00\x08\x00\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x93\x26\xDC\x72"
```

### Analyzed by `itamae`

![analysis from itamae](https://i.imgur.com/1WHDG9p.png)

```json
{"framectrl":{"vers":0,"type":2,"subtype":8,"flags":{"td":0,"fd":1,"mf":0,"r":0,"pm":0,"md":0,"pf":0,"o":0}},"present":["framectrl","duration","addr1","addr2","addr3","seqctrl","qos"],"offset":26,"stripped":0,"err":[],"duration":{"type":"vcs","dur":60},"addr1":"12:34:56:78:90:ff","addr2":"28:e3:47:42:0a:83","addr3":"28:e3:47:42:0a:83","seqctrl":{"fragno":0,"seqno":1},"qos":{"eosp":0,"a-msdu":0,"tid":6,"ack-policy":0,"txop":0}}
```