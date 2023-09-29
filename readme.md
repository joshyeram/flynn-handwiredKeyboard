# flynn
A low profile, monoblock, non-split, ortho-linear, column-staggered, 42 keys, hand-wired keyboard with oled screen encased in resin.
<br />
A session and lifetime key press counter can be stored on the EEPROM. Current implementation comes with +8 byte offset.
<p align="center">
  <img src="https://i.imgur.com/V559iy4.jpg">
  <img src="https://i.imgur.com/I4qfsyA.jpeg">
  <img src="https://i.imgur.com/4aLCbjX.jpeg">
</p>
<p align="center">
  <img src="https://media4.giphy.com/media/v1.Y2lkPTc5MGI3NjExY3B4MDQ3Zmd3MWg5MG5iMmw3amFqZGYxeDh3cjMxYXNibHF6OGc1ciZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/dXdNrNUmgMQvavaZFb/giphy.gif">
</p>
<br />



* Keyboard Maintainer: [Joshua Chung](https://github.com/joshyeram)
* Hardware Supported: *The PCBs, controllers supported*
* Hardware Availability: *Links to where you can find this hardware*

Make example for this keyboard (after setting up your build environment):

    make flynn:default

Flashing example for this keyboard:

    make flynn:default:flash

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

## Bootloader

Enter the bootloader in 3 ways:

* **Bootmagic reset**: Hold down the key at (0,0) in the matrix (usually the top left key or Escape) and plug in the keyboard
* **Physical reset button**: Briefly press the button on the back of the PCB - some may have pads you must short instead
* **Keycode in layout**: Press the key mapped to `QK_BOOT` if it is available
