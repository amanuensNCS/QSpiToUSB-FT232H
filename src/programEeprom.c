#include <ftd2xx.h>
#include <stdio.h>


int main(int argc, char const *argv[])
{

    DWORD numDevs;
    FT_STATUS ftStatus;
    FT_HANDLE ftHandle;
    ftStatus = FT_CreateDeviceInfoList(&numDevs);
    if (ftStatus == FT_OK) {
        printf("Number of devices is %d\n",numDevs);
    }
    else {
    // FT_CreateDeviceInfoList failed
    }
    FT_Open(0, &ftHandle);
    if (ftStatus != FT_OK) {
        printf("Failed to open device");
    }
    
    FT_PROGRAM_DATA ftData;
    
    char ManufacturerBuf[32];
    char ManufacturerIdBuf[16];
    char DescriptionBuf[64];
    char SerialNumberBuf[16];
    ftData.Signature1 = 0x00000000;
    ftData.Signature2 = 0xffffffff;
    ftData.Version = 0x00000005; // EEPROM structure with FT232H extensions
    ftData.Manufacturer = ManufacturerBuf;
    ftData.ManufacturerId = ManufacturerIdBuf;
    ftData.Description = DescriptionBuf;
    ftData.SerialNumber = SerialNumberBuf;
    ftStatus = FT_EE_Read(ftHandle, &ftData);
    if (ftStatus == FT_OK) {
        if (ftData.IsFT1248H) {
            printf("FT1248H is enabled\n");
        } else {
            printf("Ft1248 is not enabled \n");
        }
        printf("LSB is : %d \nClock is idle high: %d\n", ftData.FT1248LsbH, ftData.FT1248CpolH);
    // FT_EE_Read OK, data is available in ftData
    }
    else {
        printf("Failed to read eeprom data\n");
        
    // FT_EE_Read FAILED!
    }


    int c;
    printf("Enter y to flash the eeprom with settings to enable ft1248\n");
    c = getchar();
    if (c == 'y') {
        // Enable FT1248 mode
        ftData.IsFT1248H = 1;
        // Polarity of clock, important that this matches the polarity of the signal that the master chip will send.
        ftData.FT1248CpolH = 1;         
        ftData.FT1248LsbH = 0; // Enable LSB first. 0 Default.
        FT_EE_Program(ftHandle, &ftData);
    } else {
        printf("No changes were made \n");
    }
    
    FT_Close(ftHandle);

    return 0;
}
