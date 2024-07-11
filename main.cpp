#include <iostream>
#include <thread>

#include "LoraInterface.h"
#include "CloudGateWay.h"

using namespace std;

int main(int argc, char *argv[])
{
    string data;
    LoraInterface serial("/dev/ttyUSB0", 115200);
    this_thread::sleep_for(chrono::seconds(2));
    CloudGateWay client("https://cloud-gateway.vehicletracking.haipham.org");

    while (1)
    {
        
        data = serial.readStringUntil("\r\n");
        if (!data.empty())
        {
            client.sendData(data);
        }
        else
        {
            this_thread::sleep_for(chrono::seconds(2));
        }
    }

    serial.close();
    return 0;
}