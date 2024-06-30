#include "lora_interface.h"
#include <iostream>

int main(int argc, char *argv[])
{
    lora_if_init("/dev/ttyUSB0");
    lora_if_start();

    while(1){
        // some code
    }
    return 0;
}