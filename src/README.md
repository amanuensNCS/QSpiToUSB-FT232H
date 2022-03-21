# ft232h
The ft232h chip has to be configured with the eeprom on the um232h board to boot into "ft1248" mode. This is done with the "programEeprom.c" program. 
Data is then read from the ft232h chip using the other c program. 

## Dependencies
Both programs requires the ftd2xx library to be installed and linked with.
