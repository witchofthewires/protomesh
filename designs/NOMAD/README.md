# NOMAD

The Protomesh NOMAD is a portable handheld Meshtastic node that enables hyperresilient decentralized encrypted text messaging. Based on the Heltec ESP32-V4 board, itself built off the HT-RA62 LoRa chipset and the ESP-32S3 Wifi/Bluetooth chipset, it supports 915 MHz LoRa radio, 2.4GHz Wifi, and 2.4GHz Bluetooth connectivity. Capable of operating without access to cellular networks or the Internet, while also able to use MQTT over Internet to talk to nodes all over the United States today, the NOMAD is perfect for disaster recovery, emergency response, and hyperlocal community organization.

![Photograph of a NOMAD handheld node](/static/NOMAD_small.png "NOMAD")

## Hardware
The NOMAD is constructed from the following components (all prices from October 2026 and given before taxes/shipping)
- Board: [Heltec ESP32S3-V4](https://heltec.org/project/wifi-lora-32-v4/) ($19.90)
- Battery: Custom 18650 battery pack with >=2.5Ah capacity
- Antenna: [GT-800](https://heltec.org/project/gt-800-whip-antenna/) ($3.90)
- Case: 3D Printed PETG (TODO - link design)

## Software
Any working nodes distributed by the Protomesh Collective (after having been assembled, flashed, and configured by community volunteers) utilize the latest stable beta version of the Meshtastic client software (2.7.26 as of 2026 Oct 10) configured to operate in the US region (over 915 MHz).