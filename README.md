# QSpiToUSB-FT232H
Code to program a ft232h chip to act as a quad spi slave and "no HAL" example code to set up a h7 to transmit quad spi at a high rate. 

## QSpi configuration
Note that quad spi exists in a few different flavours so it is important that the ft232h and the sending chip is set up the same. The ft232h uses a protocol FTDI calls "ft1248" which can be made to work as quad spi slave but the terminology might not be the same as used in a quad spi context.

## Image of setup:

![Image of H7 connected to a ft232h chip](img/QSpiConnection.jpg)
