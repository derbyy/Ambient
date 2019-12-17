#include "SerialPort.h"


int main()
{
    unsigned int err = 0;
    const char* deviceName = "\\Device\\Silabser0";
    bool dfound = false;
    std::string comPortName;

    LedDeviceFinder* ldf = new LedDeviceFinder(deviceName);

    dfound = ldf->FindConnectedDevice(comPortName);

    if (dfound)
    {
        /* Create COM port object */
        SerialPort* port = new SerialPort(comPortName, 115200);
        /* Initialize COM port */
        err = port->initSerialPort();

        /* Do operations with COM port... */
        //err = port->writeSerialPort(buffer, strlen(buffer));
        //err = port->readSerialPort(output, sizeof(output) / sizeof(char));

        /* Release COM port object in the end */
        delete port;
    }
    else
    {
        std::cout << "Device not found." << std::endl;
    }

    delete ldf;
}
