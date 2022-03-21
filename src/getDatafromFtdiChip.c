#include <ftd2xx.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

#define PRINTERROR(msg, printEnabled) \
    if (printEnabled == 1)            \
    {                                 \
        perror(msg);                  \
    }
#define RETIFSILENT(verbose, retValue) \
    if (verbose != 1)                  \
    {                                  \
        return retValue;               \
    }

#define SWAPNIBBLES(v) ((v & 0xf0) >> 4) | ((v & 0x0f) << 4)
#define oneSecondInNS 1e9L
uint8_t lastByteRead = 0;

char RxBuffer[100000];
int readAllData(FT_HANDLE ftHandle, int verbose)
{

    FT_STATUS ftStatus = FT_OK;
    DWORD EventDWord;
    DWORD TxBytes;
    DWORD RxBytes = 500;
    DWORD BytesReceived = 0;

    ftStatus = FT_GetStatus(ftHandle, &RxBytes, &TxBytes, &EventDWord);
    if (ftStatus != FT_OK)
    {
        printf("Could not get status from device\n");
        return -1;
    }

    if (RxBytes > 0)
    {   
        //FT_SetTimeouts(ftHandle,5000,0);
        if (verbose)
            printf("Begun reading %u bytes\n", RxBytes);
        ftStatus = FT_Read(ftHandle, RxBuffer, RxBytes, &BytesReceived);

        //printf("RxBytes: %u  BytesReceived: %u\n", RxBytes, BytesReceived);
        if (ftStatus == FT_OK)
        {
            if (verbose)
                printf("Read %d number of bytes \n", BytesReceived);

            for (int i = 0; i < BytesReceived; i++)
            {
                uint8_t byteRead = (uint8_t) SWAPNIBBLES(RxBuffer[i]);

                if (byteRead != ++lastByteRead) {
                    fprintf(stderr, "MISSED VALUE, last: %u   current: %u \n", lastByteRead - 1, byteRead);
                    lastByteRead = byteRead;
                }

                if (verbose) 
                    printf("i:%u value: %u\n", i, byteRead);
            }
        }
        else
        {
            // FT_Read Failed
            perror("Failed to read from device\n");
        }
    }
    else
    {
        if (verbose)
            printf("No data to read\n");
    }
    return BytesReceived;
}

int main(int argc, char const *argv[])
{

    FT_HANDLE ftHandle;
    FT_STATUS ftStatus;
   // UCHAR latencyTimer = 10;
    DWORD InTransferSize = 64 * 1024; // Default 4096
    ftStatus = FT_Open(0, &ftHandle);
    if (ftStatus != FT_OK)
    {
        perror("Could not open device\n");
        // FT_Open failed
        return -1;
    }
    /*if (FT_SetLatencyTimer(ftHandle, latencyTimer) != FT_OK) {
        perror("Failed to set latency\n");
    }*/
    if (FT_SetUSBParameters(ftHandle, InTransferSize, 0) != FT_OK) {
        perror("Could not set usb parameters\n");
    }

    int total_read = 0;
    struct timespec ts_start, ts_curr;
    int reads_per_second = 0;
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts_start);
    while (1)
    {
        total_read += readAllData(ftHandle, 0);
        reads_per_second++;

    
        clock_gettime(CLOCK_MONOTONIC_RAW, &ts_curr);
        if ((ts_curr.tv_sec - ts_start.tv_sec) * oneSecondInNS + (ts_curr.tv_nsec - ts_start.tv_nsec) > oneSecondInNS) {
            fprintf(stdout, "Bytes per second: %d   Nr of reads done: %d\n", total_read, reads_per_second);
            clock_gettime(CLOCK_MONOTONIC_RAW, &ts_start);
            total_read = 0;
            reads_per_second = 0;
        }
    }


    FT_Close(ftHandle);
    return 0;
}
