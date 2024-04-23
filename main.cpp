#include "xboxcont.h"
#include <stdio.h>
#include <libusb-1.0/libusb.h>
#include <time.h> 
#include <unistd.h>
#include <stdlib.h>

libusb_device **devs; //
libusb_context *ctx = NULL; //
libusb_device_handle *handle = NULL;

int main() {


    //handle = malloc(sizeof(libusb_device_handle*));




   //int success;
    
    //success = open_controller(&devs, &ctx, &handle);

    int result = find_xbox_controller();

    //free(handle);

        

    libusb_free_device_list(devs, 1); // 
    libusb_exit(ctx); // 
    return 0;
}

