# AtomWire+

_Documentation can be found in the [blog for this project](http://fossil12.github.io/AtomWirePlus/) and in the [final report.](https://docs.google.com/document/d/1toPThpBCSNtpZsruHBicHuylUEN3GBtxlYa6yCWUuys/edit?usp=sharing)_

## Build instructions

You need the [Arduino IDE](https://www.arduino.cc/en/Main/Software) to flash the software on the corresponding microcontrollers.

### Master

The AtomWire+ code for the master can be found in `AtomWire_fork/AtomWireMaster`.

1. Open `AtomWireMaster.ino`.
2. Make sure your Arduino Uno is connected
3. Choose in `Tools` the Arduino Uno board and the port it is connected to the computer.
4. Upload the sketch to the board
5. Connect the one-wire line to the configured pin(s) in the sketch
6. Open the serial terminal to see the number of nodes on every line and any message that might be received

### Slave

The AtomWire+ code for the slave can be found in `AtomWire_fork/AtomWireSlave`.

1. You will need a AVR programmer. Put in the AVR Attiny85 into the programmer and plug the the programmer into the USB port of the system. 
2. Open `AtomWireSlave.ino`. 
3. Follow the steps [here](http://highlowtech.org/?p=1695) to add AVRAttiny85 as a list of components to Arduino IDE. 
4. Copy the `boards.txt` file and paste it in `Sketchbook folder > hardware > attiny > avr`
5. Choose `Tools > Processor > ATtiny85`
6. Choose `Tools > Clock > 8Mhz "Internal`
7. Choose `Tools > Programmer > USBTinyISP`
8. Select `Tools > Burn Bootloader`. This will burn the bootloader to the AVR with configuration in the `boards.txt`.
9. Change the address of the node by modifying `rom[8]` in the sketch.
9. Press upload to upload the sketch to the ATtiny. 
10. Remove the IC from the programmer and connect it along with the accompanying circuitry.