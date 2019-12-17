#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <windows.h>
#include <iostream>

class SerialPort
{
private:
    HANDLE handler;
    bool connected;
    COMSTAT status;
    DWORD errors;
    const char* portName;
    unsigned int baudRate;
public:
    SerialPort(const char* sPortName, unsigned long lBaudRate);
    ~SerialPort();

    unsigned int initSerialPort();
    int readSerialPort(char* buffer, unsigned int buf_size);
    bool writeSerialPort(char* buffer, unsigned int buf_size);
    bool isConnected();
    void closeSerial();
};

#endif // SERIALPORT_H

