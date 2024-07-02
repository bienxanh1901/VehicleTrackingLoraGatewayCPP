#pragma once

#include <termios.h>
#include <vector>
#include <mutex>
#include <string>

class LoraInterface
{
public:
    static LoraInterface *instance();
    std::string readMessage(const std::string delim="\r\n");
    bool isReady();
    void openSerial(std::string port, int baudrate = B115200);
    void closeSerial();
    void readSerial();

private:
    static std::vector<char>::iterator findStringInVector(std::vector<char>& v,const std::string& s);

private:
    int m_fd = 0;
    struct termios m_tty;
    std::vector<char> m_buffer;
    std::mutex m_mutex;
    bool m_ready = false;
};

