## Installation

```
make
sudo insmod asoc-i2s-loader.ko
```

Afterwards check `dmesg` for any errors. If `arecord -l` lists your device,
copy asoc-i2s-loader.ko to modules).

## Compatibility

This module has been tested with Compute Module and INMP441 MEMS microphone.
