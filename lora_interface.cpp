#include "lora_interface.h"

#include <iostream>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/signal.h>
#include <errno.h>

void signal_handler_IO(int status); /* definition of signal handler */

int fd;
struct termios tty;
struct sigaction saio;
std::string buf;
int i = 0;

bool lora_if_init(const char *port, int baudrate)
{
    fd = open(port, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1)
    {
        return false;
    }

    fcntl(fd, F_SETFL, FNDELAY);
    fcntl(fd, F_SETOWN, getpid());
    fcntl(fd, F_SETFL, O_ASYNC);

    tcgetattr(fd, &tty);
    cfsetispeed(&tty, baudrate);
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;
    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_oflag &= ~OPOST;
    tcsetattr(fd, TCSANOW, &tty);

    return true;
}

void lora_if_start()
{
    saio.sa_handler = signal_handler_IO;
    saio.sa_flags = 0;
    saio.sa_restorer = nullptr;
    sigaction(SIGIO, &saio, nullptr);
}

void lora_if_close()
{
    close(fd);
}

void signal_handler_IO(int status)
{
    char tmp[64] = { 0 };
    int n = read(fd, tmp, 64);
    std::string str(tmp);
    int pos = str.find_last_of("\n");
    if (pos == std::string::npos)
    {
        buf += tmp;
    }
    else
    {
        buf += str.substr(0, pos);
        
        std::cout << buf.length() << "," << buf << std::endl;
        buf = str.substr(pos + 1);
    }
}