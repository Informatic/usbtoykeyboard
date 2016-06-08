USB Toy MIDI Keyboard
=====================

Simple "retrofit" (or rather... toyfit?) of a simple toy musical keyboard based
on ATMega 32u4 (Arduino Leonardo / Arduino Pro Micro and such) with USB MIDI and
(old "DIN5") MIDI output.

Requirements
------------
Project requires [arcore](https://github.com/rkistner/arcore) for proper USB
MIDI support. (If you don't need that, it's pretty simple to rip it out)

Wiring & configuration
----------------------
Pretty much all toy keyboards are [simple key matrices without
diodes](http://pcbheaven.com/wikipages/How_Key_Matrices_Works/). Columns and
rows pins are defined in `colsPins` and `rowsPins` arrays. `midiBaseNote` is
MIDI Note value for first keyboard in a matrix. `midiChannel` is MIDI Channel
notes will be sent to. `COLS_COUNT` and `ROWS_COUNT` are self-explanatory.
**Note:** Code assumes `COLS_COUNT` to not be higher than 8.

TODO
----
 * Key ghosting needs to be handled gracefuly
 * Encoder / potentiometer for CC


Disclaimer
----------
I wrote it quite a long time ago, and just posting it now ;)

License
-------
MIT
