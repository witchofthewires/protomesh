# HACKERSBLOOM Build Guide
![Photograph of a disassembled HACKERSBLOOM solar node (solar panel not pictured)](/static/HACKERSBLOOM/HB000_internal_nosolar_small.jpg "Inside the HACKERSBLOOM")

## Tools You Will Need
- Screwdriver (Phillips #1 or #0 fits best)
- Cutters
- Soldering Iron + Solder
- Silicone Sealant
- Drill 
- 1/4" Drill Bit
    - Optional: Larger Bit (ex. 3/8")

## Materials
- [Harbor Breeze 1W Solar Light](https://www.lowes.com/pd/Harbor-Breeze-60-Lumens-1-Watt-Black-Warm-White-Solar-LED-Spot-light/5018244381)
- [Heltec T114](https://heltec.org/project/mesh-node-t114/?attribute_pa_band=902-928mhz&attribute_need-display=No&attribute_need-gps=No&attribute_need-shell=No)
- [Antenna Cable](https://heltec.org/project/ipex1-to-sma-antenna-feeder-cable-rf113-10mm/?attribute_antenna-feeder-type=SMA+Female+Straight+Connector%2810mm%29)
- [Antenna](https://heltec.org/project/gt-800-whip-antenna/)
- 2 x JST Cable (These should come with the T114)

## Quick assembly instructions:
1. Obtain a [Harbor Breeze 1W Solar Light](https://www.lowes.com/pd/Harbor-Breeze-60-Lumens-1-Watt-Black-Warm-White-Solar-LED-Spot-light/5018244381) (item #1234868) from Lowe's
2. Open it up, remove battery, and snip the cables (LED+/-, S+/-, BAT-)
3. Unscrew the circuit board and remove the light
4. Clip the circuit board between the switch and tiny black integrated circuit
5. De-solder the wires from the solar panel and replace them with a JST cable (triple check polarity!)
6. Solder second JST connector to battery/board (triple check polarity!)
7. Drill 1/4" hole for antenna
8. Add sealant and install antenna cable
9. Install antenna
10. Plug in battery and solar cables
11. Put in battery
12. Screw the circuit board back in
13. Screw case back together
14. Remove sticker from solar panel

## Detailed Instructions

#### 1. Obtain a [Harbor Breeze 1W Solar Light](https://www.lowes.com/pd/Harbor-Breeze-60-Lumens-1-Watt-Black-Warm-White-Solar-LED-Spot-light/5018244381) (item #1234868) from Lowe's
<table>
<tr><td>

<img alt="Photograph of a Lowe's solar light sitting on a desk" title="Lowe's Harbor Breeze 1W Solar Light" width=320 src="/static/HACKERSBLOOM/lowes_hb_light.jpg">

</td><td>
During the period of active HACKERSBLOOM development (April 2026 to October 2026/present as of writing), this light is typically on sale for $10 and has been observed as low as $5.
<br><br>
This tutorial is specific to this model of solar light; DO NOT use a different solar light for this tutorial unless you are absolutely confident that you have modified the circuit as necessary to prevent shorts and ensure proper functionality.
</td></tr>
</table>

#### 2. Open it up, remove the battery, and snip the cables (everything except for the BAT- connection)
<table>
<tr>
    <td><img align="left" src="/static/HACKERSBLOOM/HB003_open.jpg" width="320"></td>
    <td><img align="right" src="/static/HACKERSBLOOM/HB004_opened.jpg" width="320"></td>
</tr>
<tr>
    <td><img align="left" src="/static/HACKERSBLOOM/HB005_remove_battery.jpg" width="320"></td>
    <td><img align="right" src="/static/HACKERSBLOOM/HB006_cut_wires.jpg" width="320"></td>
</tr>
</table>

#### 3. Unscrew the circuit board and remove the light
<table>
<tr>
    <td><img align="left" src="/static/HACKERSBLOOM/HB007_unscrew_board.jpg" width="320"></td>
</tr>
<tr>
    <td><img align="left" src="/static/HACKERSBLOOM/HB008_unscrew_light.jpg" width="320"></td>
    <td><img align="right" src="/static/HACKERSBLOOM/HB009_remove_light.jpg" width="320"></td>
</tr>
</table>

#### 4. Clip the circuit board between the switch and tiny black integrated circuit
<table>
<tr>
    <td><img align="left" src="/static/HACKERSBLOOM/HB010_cut_position_1.jpg" width="320"></td>
    <td><img align="right" src="/static/HACKERSBLOOM/HB011_cut_position_2.jpg" width="320"></td>
</tr>
<tr>
    <td><img align="center" src="/static/HACKERSBLOOM/HB012_cut_board.jpg" width="320"></td>
</tr>
</table>

#### 5. De-solder the wires from the solar panel and replace them with a JST cable (triple check polarity!)
<table>
<tr>
    <td><img align="center" src="/static/HACKERSBLOOM/HB013_desolder_solar.jpg" width="320"></td>
</tr>
<tr>
    <td>
        <img align="left" src="/static/HACKERSBLOOM/HB014_bend_jst.jpg" width="320">
        Bending the ends of the wires can make it easier to hold them in place.
    </td>
    <td><img align="right" src="/static/HACKERSBLOOM/HB015_solder_solar.jpg" width="320"></td>
</tr>
</table>

#### 6. Solder second JST connector to battery/board (triple check polarity!)
<table>
<tr>
    <td><img align="left" src="/static/HACKERSBLOOM/HB016_solder_battery_positive.jpg" width="320"></td>
    <td><img align="right" src="/static/HACKERSBLOOM/HB017_solder_battery_negative.jpg" width="320"></td>
</tr>
<tr>
    <td><img align="center" src="/static/HACKERSBLOOM/HB018_finished_soldering.jpg" width="320"></td>
</tr>
</table>

#### 7. Drill 1/4" hole for antenna
<table>
<tr>
    <td>
        <img align="left" src="/static/HACKERSBLOOM/HB019_easy_drill_spot.jpg" width="320">
        The molding mark on the back of the light can act as a nice center hole for drilling.
    </td>
    <td>
        <img align="right" src="/static/HACKERSBLOOM/HB020_quarter_inch_hole.jpg" width="320">
        Drill out the hole with a 1/4" bit.
    </td>
</tr>
<tr>
    <td>
        <img align="center" src="/static/HACKERSBLOOM/HB021_debur_hole.jpg" width="320">
        Optional: You can de-bur the hole by lightly spinning a larger drill bit (ex. 3/8") by hand around the hole. 
    </td>
</tr>
</table>

#### 8. Add sealant and install antenna cable
<table>
<tr>
    <td>
        <img align="left" src="/static/HACKERSBLOOM/HB022_apply_sealant.jpg" width="320">
        Apply sealant around hole, a toothpick or q-tip can be very helpful.
    </td>
    <td>
        <img align="right" src="/static/HACKERSBLOOM/HB023_insert_antenna_cable.jpg" width="320">
        Insert antenna cable into hole.
    </td>
</tr>
<tr>
    <td>
        <img align="left" src="/static/HACKERSBLOOM/HB024_clean_threads.jpg" width="320">
        Make sure to clean any sealant off of the threads of the connector with a paper towel.
    </td>
    <td>
        <img align="right" src="/static/HACKERSBLOOM/HB025_washer_nut_stack.jpg" width="320">
        Install the toothed washer, then the spring washer, and finally the nut. Tighten completely.
    </td>
</tr>
</table>

#### 9. Install antenna
<table>
<tr>
    <td>
        <img align="center" src="/static/HACKERSBLOOM/HB026_antenna.jpg" width="320">
        Screw on the antenna.
    </td>
</tr>
<tr>
    <td>
        <img align="left" src="/static/HACKERSBLOOM/HB027_antenna_cable.jpg" width="320">
        Be careful with this connector, it is pretty delicate.
    </td>
    <td>
        <img align="right" src="/static/HACKERSBLOOM/HB028_antenna_connected.jpg" width="320">
        Connect the antenna cable to the T114.
    </td>
</tr>
</table>

#### 10. Plug in battery and solar cables
<table>
<tr>
    <td>
        <img align="left" src="/static/HACKERSBLOOM/HB029_battery_connected.jpg" width="320">
        Plug in the battery cable, ensuring the polarity matches
    </td>
    <td>
        <img align="right" src="/static/HACKERSBLOOM/HB030_solar_connected.jpg" width="320">
        Plug in the solar cable, ensuring the polarity matches
    </td>
</tr>
</table>

#### 11. Put in battery

#### 12. Screw the circuit board back in

#### 13. Screw case back together
<table>
<tr>
    <td><img align="center" src="/static/HACKERSBLOOM/HB031_screwed_together.jpg" width="320"></td>
</tr>
</table>

#### 14. Remove sticker from solar panel
