#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>

enum Devices { DEVICE_ESP = 0, DEVICE_COUNT };

class SerialPort
{
private:
    HANDLE m_handler;
    bool m_connected;
    COMSTAT m_status;
    DWORD m_errors;
    std::string m_portName;
    unsigned int m_baudRate;
    std::map<std::string, std::string> m_ports;
    std::map<Devices, std::string> m_devices;

    const std::string supported_devices[DEVICE_COUNT] = { "Silabser0" };
public:
    SerialPort(unsigned long lBaudRate);
    ~SerialPort();

    unsigned int initSerialPort();
    int readSerialPort(char* buffer, unsigned int buf_size);
    bool writeSerialPort(char* buffer, unsigned int buf_size);
    bool isConnected();

private:
    std::map<std::string, std::string> FindConnectedDevices();
    bool IsSupported(std::map<std::string, std::string> &devices);
};

#endif // SERIALPORT_H

