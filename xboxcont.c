#include <stdio.h>
#include <libusb-1.0/libusb.h>
#include <time.h> 
#include <unistd.h>

#define SLEEP 1

int globalX = 5;
int globalY = 5;


void processDirection(unsigned char data) {
    int twiceTest=1;

    if (data == 0x00) {
        printf("data[2] is '00'\n");
    } else {
        if (twiceTest==1){
            twiceTest=0;
            switch (data) {
                case 0x01:  // up
                    printf("got '01'\n");
                    globalY = globalY+1;
                    printf("The value of the coordinate is: %d %d\n", globalX,globalY); 
                    sleep(SLEEP);   // sleep 1s
                    break;
                case 0x08:  // right
                    printf("got '08'\n");
                    globalX = globalX+1;
                    printf("The value of the coordinate is: %d %d\n", globalX,globalY); 
                    sleep(SLEEP);   // sleep 1s
                    break;
                case 0x02:  // down
                    printf("got '02'\n");
                    globalY = globalY-1;
                    printf("The value of the coordinate is: %d %d\n", globalX,globalY);
                    sleep(SLEEP);   // sleep 1s
                    break;
                case 0x04:  // left
                    printf("got '04'\n");
                    globalX = globalX-1;
                    printf("The value of the coordinate is: %d %d\n", globalX,globalY);
                    sleep(SLEEP);   // sleep 1s
                    break;
                default:
                    printf("unknoWN\n");
                    break;
            }

        }else{
            //twiceTest此时等于0
            twiceTest=1;
        }
        // 如果不是 '00'，进入 switch 语句处理其它情况

    }
}

void processFunction(unsigned char data){
    //data[3]: Y:80 B:20 A:10 X:40 LB:01 RB:02
    int twiceTestFunc=1;
    if (twiceTestFunc==1){
        twiceTestFunc=0;
        switch (data) {
            case 0x80:  // Y
                printf("got Y\n");
                sleep(SLEEP);   // sleep 1s
                break;
            case 0x20:  // B
                printf("got B\n");
                sleep(SLEEP);   // sleep 1s
                break;
            case 0x10:  // A
                printf("got A\n");
                printf("Send coordinate to AI: %d %d\n", globalX,globalY);
                sleep(SLEEP);   // sleep 1s
                break;
            case 0x40:  // X
                printf("got X\n");
                sleep(SLEEP);   // sleep 1s
                break;
            default:
                printf("unknoWN\n");
                break;
        }    
    }else{
        twiceTestFunc=1;
    }


}



void printInput(unsigned char arr[], int size) {
    //read direction input data[2]: up:01 r:08 d:02 l:04
    if (arr[2] != 0x00) {
        processDirection(arr[2]);
    }
    if (arr[3] != 0x00){
        processFunction(arr[3]);
    }
}

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
