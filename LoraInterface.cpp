#include "LoraInterface.h"

#include <iostream>
#include <algorithm>
#include <functional>
#include <memory>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/signal.h>
#include <errno.h>

using namespace std;
using namespace std::placeholders;

static struct sigaction saio;

void signalIoHandler(int status)
{
    LoraInterface::instance()->readSerial();
}


LoraInterface *LoraInterface::instance()
{
    static LoraInterface *loraInterface = nullptr;

    if (loraInterface == nullptr)
    {
        loraInterface = new LoraInterface();
    }

    return loraInterface;
}

void LoraInterface::openSerial(string port, int baudrate)
{
    m_ready = false;
    m_fd = open(port.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if (m_fd == -1)
    {
        return;
    }

    fcntl(m_fd, F_SETFL, FNDELAY);
    fcntl(m_fd, F_SETOWN, getpid());
    fcntl(m_fd, F_SETFL, O_ASYNC);

    tcgetattr(m_fd, &m_tty);
    cfsetispeed(&m_tty, baudrate);
    m_tty.c_cflag &= ~PARENB;
    m_tty.c_cflag &= ~CSTOPB;
    m_tty.c_cflag &= ~CSIZE;
    m_tty.c_cflag |= CS8;
    m_tty.c_cflag |= (CLOCAL | CREAD);
    m_tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    m_tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    m_tty.c_oflag &= ~OPOST;
    tcsetattr(m_fd, TCSANOW, &m_tty);

    saio.sa_handler = signalIoHandler;
    saio.sa_flags = 0;
    saio.sa_restorer = nullptr;
    sigaction(SIGIO, &saio, nullptr);

    cout << "serial opened" << endl;
    m_ready = true;
}

void LoraInterface::closeSerial()
{
    close(m_fd);
    cout << "serial closed" << endl;
}

bool LoraInterface::isReady()
{
    return m_ready;
}

void LoraInterface::readSerial()
{
    if (!m_ready) return;
    char buf[64] = { 0 };
    int n = read(m_fd, buf, 64);
    
    lock_guard<mutex> lock(m_mutex);
    m_buffer.insert(m_buffer.end(), buf, buf + n);
}

std::string LoraInterface::readMessage(const std::string delim)
{
    lock_guard<mutex> lock(m_mutex);
    vector<char>::iterator it=findStringInVector(m_buffer, delim);

    if(it==m_buffer.end()) return "";
    string result(m_buffer.begin(),it);
    it+=delim.size();//Do remove the delimiter from the queue
    m_buffer.erase(m_buffer.begin(),it);
    return result;
}

std::vector<char>::iterator LoraInterface::findStringInVector(std::vector<char>& v,const std::string& s)
{
    if(s.size()==0) return v.end();

    vector<char>::iterator it=v.begin();
    for(;;)
    {
        vector<char>::iterator result=find(it,v.end(),s[0]);
        if(result==v.end()) return v.end();//If not found return
        for(size_t i=0;i<s.size();i++)
        {
            vector<char>::iterator temp=result+i;
            if(temp==v.end()) return v.end();
            if(s[i]!=*temp) goto mismatch;
        }
        //Found
        return result;

        mismatch:
        it=result+1;
    }
}

