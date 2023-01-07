# Mobility Adapter PCB (v3) firmware configuration procedure

This configuration process guides you through preparing hardware and configuring firmware for the Mobility-Adapter-v3 PCB as of late 2022.  

## Cut redundant traces on PCB (v3).

The pin next to VIN on the ESP32 Module footprint, Labelled GND, is a mistake. The pis should not be connected to ground. 
1. Cut the traces for that pin, connecting it to the ground plane.

2. Check that the pin is isolated using a multimeter between the isolated pin and any other GND point.

## Adjust the Buck Converter Voltage

Once the main motor driver header, fuse and buck module are soldered, the output of the buck can be set to 5v.

1. Connect the 12-30v supply voltage to the B+ and B-. NOTE. This board doesn't have reverse polarity protection. DOUBLE CHECK POLARITY.

2. Power up the board and adjust the trim pot on the buck converter to 5.0 - 5.1V. The 5V and GNC pins associated with the relays header are convenient test points.

## Setup Bluetooth Controller 

1. Flash the Ps3Address.ino file to the ESP32. it can be found at;

`firmware\PS3Controller\Ps3Address\Ps3Address.ino`

2. install and run the 'SixAxisPairTool' and enter the MAC address discovered in the previous step.

3. test the pairing using the Ps3Test-Friendly.ino sketch, found at;

`firmware\PS3Controller\Ps3Test-Friendly\Ps3Test-Friendly.ino`

It's necessary to modify the folloing line in the setup function, substituting in the correct mac address for this particular pairing.

`Ps3.begin("xx:xx:xx:xx:xx:xx");`

## Configure Firmware for Specific Platform

While Each platform will need a custom configuration regarding the use of the output, all platforms will require the following modifications.