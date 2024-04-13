#include <stdio.h>
#include <libusb-1.0/libusb.h>

int main() {
    libusb_device **devs; //
    libusb_context *ctx = NULL; //
    libusb_device_handle *handle = NULL;
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
        
    unsigned char endpoint_address = 0x82; // 端点地址
    unsigned char data[64]; // 数据缓冲区
    int actual_length; // 实际读取的数据长度
    int timeout = 5000; // 超时时间，以毫秒为单位

    while (1) {
        // 使用libusb_bulk_transfer读取数据
        rr = libusb_interrupt_transfer(handle, endpoint_address, data, sizeof(data), &actual_length, timeout);
        if (rr == 0) {
            printf("read success: ");
            for (int i = 0; i < actual_length; ++i) {
                printf("%02x ", data[i]);
            }
            printf("\n");
        } else {
            fprintf(stderr, "fail to read: %d\n", r);
            break; //�
        }
    }
    
    libusb_free_device_list(devs, 1); // 
    libusb_exit(ctx); // 
    return 0;
}
