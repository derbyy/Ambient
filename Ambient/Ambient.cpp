#include "SerialPort.h"

int main()
{
    unsigned int err = 0;
    const char* comPortName = "\\\\.\\COM5";

    char buffer[6] = "test\n";
    char output[6] = {'\0'};

    SerialPort* port = new SerialPort(comPortName, 115200);

    err = port->initSerialPort();

    err = port->writeSerialPort(buffer, strlen(buffer));

    err = port->readSerialPort(output, sizeof(output) / sizeof(char));

    delete port;
}
