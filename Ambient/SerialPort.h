#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <windows.h>
#include <iostream>
#include <string>

class SerialPort
{
private:
    HANDLE handler;
    bool connected;
    COMSTAT status;
    DWORD errors;
    std::string portName;
    unsigned int baudRate;
public:
    SerialPort(std::string sPortName, unsigned long lBaudRate);
    ~SerialPort();

    unsigned int initSerialPort();
    int readSerialPort(char* buffer, unsigned int buf_size);
    bool writeSerialPort(char* buffer, unsigned int buf_size);
    bool isConnected();
    void closeSerial();
};


class LedDeviceFinder
{
private:
    const char* deviceName;

public:
    LedDeviceFinder(const char* sDeviceName);
    ~LedDeviceFinder();

    bool FindConnectedDevice(std::string &data);
};

#endif // SERIALPORT_H

