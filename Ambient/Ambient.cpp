#include "SerialPort.h"


#include <chrono>

class Timer
{
public:
    Timer()
    {
        m_StartTimepoint = std::chrono::high_resolution_clock::now();
    }

    ~Timer()
    {
        Stop();
    }

    void Stop()
    {
        auto endTimepoint = std::chrono::high_resolution_clock::now();
        auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
        auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

        auto duration = end - start;
        std::cout << "Function takes : " << duration << " us" << std::endl;
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
};







int main()
{













    //unsigned int err = 0;
    //const char* deviceName = "\\Device\\Silabser0";
    //bool dfound = false;
    //std::string comPortName;

    //LedDeviceFinder* ldf = new LedDeviceFinder(deviceName);

    //dfound = ldf->FindConnectedDevice(comPortName);

    //if (dfound)
    //{
    //    /* Create COM port object */
    //    SerialPort* port = new SerialPort(comPortName, 115200);
    //    /* Initialize COM port */
    //    err = port->initSerialPort();

    //    /* Do operations with COM port... */
    //    //err = port->writeSerialPort(buffer, strlen(buffer));
    //    //err = port->readSerialPort(output, sizeof(output) / sizeof(char));

    //    /* Release COM port object in the end */
    //    delete port;
    //}
    //else
    //{
    //    std::cout << "Device not found." << std::endl;
    //}

    //delete ldf;
}
