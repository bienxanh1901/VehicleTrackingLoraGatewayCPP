#pragma once

#include <termios.h>

bool lora_if_init(const char *port, int baudrate = B115200);
void lora_if_start();
void lora_if_close();

