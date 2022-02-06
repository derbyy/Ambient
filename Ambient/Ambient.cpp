#include "SerialPort.h"

int main()
{
    unsigned int err = 0;
    char buffer[5] = { '\0' };
    char out_buffer[50] = { '\0' };

    /* Create COM port object */
    SerialPort port(115200);

    /* Initialize COM port */
    err = port.initSerialPort();

    if (err == 0)
    {
        buffer[0] = 'a';

        /* Do operations with COM port... */
        err = port.writeSerialPort(buffer, sizeof(buffer) / sizeof(char));
        err = port.readSerialPort(out_buffer, sizeof(out_buffer) / sizeof(char));
    }
}
