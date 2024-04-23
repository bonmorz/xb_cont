#include "xboxcont.h"
#include <stdio.h>
#include <libusb-1.0/libusb.h>
#include <time.h> 
#include <unistd.h>

int main() {
    libusb_device **devs; //
    libusb_context *ctx = NULL; //
    libusb_device_handle *handle = NULL;

    handle = malloc(sizeof(libusb_device_handle*));




    int success;
    
    success = open_controller(&devs, &ctx, &handle);

    free(handle);

        

    libusb_free_device_list(devs, 1); // 
    libusb_exit(ctx); // 
    return 0;
}

