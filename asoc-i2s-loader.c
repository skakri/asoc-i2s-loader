#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kmod.h>
#include <linux/platform_device.h>
#include <sound/simple_card.h>
#include <linux/delay.h>

/*
    Inspired by https://github.com/msperl/spi-config
    with thanks for https://github.com/notro/rpi-source/wiki
    as well as Florian Meier for the rpi i2s and dma drivers.

    To use a different (simple-card compatible) codec
    change the codec name string in two places and the
    codec_dai name string. (see codec's source file).

    fmt flags are set for vanilla i2s with rpi as clock master.

    N.B. playback vs capture is determined by the codec choice.

    Updated for kernel 4.1+ with the help of carla_sch:
    https://www.raspberrypi.org/forums/viewtopic.php?f=44&t=91237#p803012
*/

void device_release_callback(struct device *dev) { /* do nothing */ };

static struct asoc_simple_card_info snd_rpi_simple_card_info = {
  .card = "snd_rpi_simple_card", // -> snd_soc_card.name
  .name = "simple-card_codec_link", // -> snd_soc_dai_link.name
  // .codec = "dmic-codec"?
  .codec = "snd-soc-dummy", // -> snd_soc_dai_link.codec_name
  .platform = "20203000.i2s", // -> snd_soc_dai_link.platform_name
  // Investigate, if we don't need to keep SND_SOC_DAIFMT_CBS_CFS instead
  // of SND_SOC_DAIFMT_CBS_CFM.
  .daifmt = SND_SOC_DAIFMT_I2S | SND_SOC_DAIFMT_NB_NF | SND_SOC_DAIFMT_CBS_CFM,
  .cpu_dai = {
    .name = "20203000.i2s", // -> snd_soc_dai_link.cpu_dai_name
    .sysclk = 0
  },
  .codec_dai = {
    // .name = "dmic-hifi"?
    .name = "snd-soc-dummy-dai", // -> snd_soc_dai_link.codec_dai_name
    .sysclk = 0
  },
};

static struct platform_device snd_rpi_simple_card_device = {
  .name = "asoc-simple-card", // Module alias.
  .id = 0,
  .num_resources = 0,
  .dev = {
    .release = &device_release_callback,
    .platform_data = &snd_rpi_simple_card_info, // *HACK ALERT*
  },
};

static struct platform_device snd_rpi_codec_device = {
  .name = "dmic-codec", // Module alias.
  .id = -1,
  .num_resources = 0,
  .dev = {
    .release = &device_release_callback,
  },
};

int hello_init(void) {
  const char *dmaengine = "bcm2708-dmaengine"; // module name.
  int ret;

  ret = request_module(dmaengine);
  if (ret)
    pr_alert("Unable to request module load '%s': %d\n",
             dmaengine, ret);

  ret = platform_device_register(&snd_rpi_codec_device);
  if (ret)
    pr_alert("Unable to register platform device '%s': %d\n",
             snd_rpi_codec_device.name, ret);

  ret = platform_device_register(&snd_rpi_simple_card_device);
  if (ret)
    pr_alert("Unable to register platform device '%s': %d\n",
             snd_rpi_simple_card_device.name, ret);

  pr_alert("ASoC simple-card I2S loader initialized\n");
  return 0;
}

void hello_exit(void) {
  // You'll have to sudo modprobe -r the card & codec drivers manually (first?)
  platform_device_unregister(&snd_rpi_simple_card_device);
  platform_device_unregister(&snd_rpi_codec_device);
  pr_alert("ASoC simple-card I2S loader unloaded\n");
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_DESCRIPTION("ASoC simple-card I2S setup");
MODULE_AUTHOR("Plugh Plover");
MODULE_LICENSE("GPL v2");
