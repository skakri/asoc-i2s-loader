# Setup steps

Note: may not be 100% inaccurate. Proceeed with caution and common sense.

## Module blacklist
Modify `/etc/modprobe.d/raspi-blacklist.conf`:

```
blacklist snd_soc_wm8804
blacklist snd_soc_pcm512x
blacklist snd_soc_tas5713
```

## Boot configuration

Edit `/boot/config.txt`

Enable I²S: `dtparam=i2c_arm=on,i2s=on,spi=on`
Don't load `snd_bcm2835` by default: `# dtparam=audio=on`

## Set up modules

Edit `/etc/modules`, replace with following contents:

```
i2c-bcm2708
i2c-dev
# asoc-i2s-loader
```

## Build asoc-i2s-loader

Clone this repo and run build. Afterwards try to load your new module with
`insmod asoc-i2s-loader.ko`, check `dmesg` and `arecord -l` outputs.

If everything looks ok, proceed with `cp asoc-i2s-loader.ko /lib/modules/4.4.9+/kernel/sound/soc/codecs/asoc-i2s-loader.ko`
(replace 4.4.9+ with your kernel version), run `depmod -a` and uncomment
`asoc-i2s-loader` from `/etc/modules`. Then reboot.
