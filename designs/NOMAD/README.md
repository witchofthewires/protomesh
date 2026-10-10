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

## Version History
### Stable Beta Release
- 1.1.2: Updated case design to physically clip shut
- 1.1.1: Replaced adhesive-joined battery pack design (held together by pressure from case in v1.0.0 which v1.0.0 case is incapable of providing) with spot-welded battery packs
- 1.1.0: Upgraded board from Heltec ESP-32V3, requiring new case design
- 1.0.0: Initial working build

### Unstable Alpha Research
 - 1.2.0-ALPHA: Replaces GT-800 antenna with RP-SMA connector with DIY Steven's Stinger Gen2 antenna