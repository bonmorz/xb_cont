#include "xboxcont.h"
#include <stdio.h>
#include <libusb-1.0/libusb.h>
#include <time.h> 
#include <unistd.h>

int main() {
    libusb_device **devs; //
    libusb_context *ctx = NULL; //
    libusb_device_handle *handle = NULL;

    libusb_device_handle **handle = malloc(sizeof(libusb_device_handle*));


    int r; // 
    ssize_t cnt; // 
    int rr;

    r = libusb_init(&ctx); // 
    if (r < 0) {
        fprintf(stderr, "fail to init libusb: %d\n", r);
        return 1;
    }
    libusb_set_debug(ctx, 3); // 

    cnt = libusb_get_device_list(ctx, &devs); // 
    if (cnt < 0) {
        fprintf(stderr, "fail to get device list\n");
        return 1;
    }
    printf("find %zd devices\n", cnt);

    libusb_device *device;
    for (ssize_t i = 0; i < cnt; i++) {
        device = devs[i];
        struct libusb_device_descriptor desc;
        libusb_get_device_descriptor(device, &desc);


        //4.13:028e is the pid of beitong controller
        if (desc.idVendor == 0x045e && desc.idProduct == 0x028e) {
            printf("find Xbox ：VID 0x%04x, PID 0x%04x\n", desc.idVendor, desc.idProduct);
            //
            r = libusb_open(devs[i], &handle);
            if (r != LIBUSB_SUCCESS) {
                fprintf(stderr, "fail to open xbox\n");
                continue;
            } else{
                printf("success open xbox controller");

                //try to detatch kernel driver
                if (libusb_kernel_driver_active(handle, 0) == 1) { 
                    r = libusb_detach_kernel_driver(handle, 0);
                    if (r == 0) {
                        printf("Kernel Driver Detached\n");
                    } else {
                        fprintf(stderr, "Error detaching kernel driver: %d\n", r);
                    }
                }

            } 
        }
    }




    int success;
    
    success = open_controller(&devs, &ctx, &handle);

    free(handle);

        

    libusb_free_device_list(devs, 1); // 
    libusb_exit(ctx); // 
    return 0;
}

