#include <iostream>
#include <thread>

#include "LoraInterface.h"
#include "CloudGateWay.h"

using namespace std;

int main(int argc, char *argv[])
{
    string data;
    LoraInterface *serial = LoraInterface::instance();
    // CloudGateWay client("https://cloud-gateway.vehicletracking.haipham.org");
    CloudGateWay client("127.0.0.1:8888");
    serial->openSerial("/dev/ttyUSB0");
    if(!serial->isReady()) return -1;

    while (1)
    {
        
        data = serial->readMessage();
        if (!data.empty())
        {
            cout << data.length() << ", " << data << endl;
            client.sendData(data);
        }
        else
        {
            this_thread::sleep_for(chrono::seconds(2));
        }
    }

    serial->closeSerial();

    return 0;
}