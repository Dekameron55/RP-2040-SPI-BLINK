#RP2040 SPI and Verilog SPI module Communication example

1. This is an example of how the RP2040 communicates via SPI to a custom Verilog module.

The Verilog module takes the LSB of the sent byte and sets the bit either 1 or 0 to turn on or off the LED
The Verilog module transmits the number of received messages on every received byte sent by the RP2040.
The Verilog module needs a 10MHz oscilator to oversample the SPI Bus in order to detect rising edges on the bus.
The RP2040 sends bytes in a loop which controll the LED and receives the number of received messages by the slave
and outputs them to the console.

![Screenshot](https://github.com/Dekameron55/RP-2040-SPI-BLINK/blob/main/images/SetupWokWi.jpg)


Disclaimer
This source code and information is provided as is, without any warranties or guarantees of performance. The author assumes no responsibility for any issues arising from its use in any system. Users should verify and test the module thoroughly before deploying it in critical applications.