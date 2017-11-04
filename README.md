# Interledger of Lasers
> 'nuff said.

## Schematics

### Sensor

The phototransistor should be an ordinary phototransistor, and the transistor
reading "NPN" should actually be a TIP120. I also used an IN4007 diode instead of an IN4004,
but both should work fine. The power supply should be 12V instead of 9V.

![Sensor](./schematics/IOL_Sensor_bb.svg)

### Transmitter

The LED should be replaced with a laser.

![Transmitter](./schematics/IOL_Transmitter_bb.svg)
