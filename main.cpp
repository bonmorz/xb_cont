#include "xboxcont.h"
#include <stdio.h>
#include <libusb-1.0/libusb.h>
#include <time.h> 
#include <unistd.h>

int main() {
    libusb_device **devs; //
    libusb_context *ctx = NULL; //
    libusb_device_handle *handle = NULL;

    int success;
    
    success = open_controller(&devs, &ctx, &handle);

        
    unsigned char endpoint_address = 0x81; // endpoint set to 0x81
    unsigned char data[20]; // 数据缓冲区
    int actual_length; // 实际读取的数据长度
    int timeout = 5000; // 超时时间，以毫秒为单位


    while (1) {
        // 使用libusb_bulk_transfer读取数据
        // actually we keep reading this even if we have no input
        // 
        rr = libusb_interrupt_transfer(handle, endpoint_address, data, sizeof(data), &actual_length, timeout);
        if (rr == 0) {
            //so we will keep use printInput

            printInput(data,actual_length);
        } else {
            fprintf(stderr, "fail to read: %d\n", r);
            break; //�
        }
    }
    
    libusb_free_device_list(devs, 1); // 
    libusb_exit(ctx); // 
    return 0;
}

