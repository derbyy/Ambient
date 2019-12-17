#include "SerialPort.h"

SerialPort::SerialPort(std::string sPortName, unsigned long lBaudRate)
{
    this->handler = nullptr;
    this->connected = false;
    this->errors = 0;
    this->baudRate = lBaudRate;
    this->portName = sPortName;
    this->status = { 0 };  
}

SerialPort::~SerialPort()
{
    if (this->connected) 
    {
        this->connected = false;
        CloseHandle(this->handler);
    }

    this->handler = nullptr;
}

unsigned int SerialPort::initSerialPort()
{
    this->handler = CreateFileA(static_cast<LPCSTR>(portName.c_str()), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (this->handler == INVALID_HANDLE_VALUE)
    {
        if (GetLastError() == ERROR_FILE_NOT_FOUND)
        {
            std::cout << "ERROR: Handle was not attached.Reason : " + portName + "not available." << std::endl;
            return 1;
        }
        else
        {
            std::cout << "Error !!" << std::endl;
            return 2;
        }
    }
    else
    {
        DCB dcbSerialParameters = { 0 };

        if (!GetCommState(this->handler, &dcbSerialParameters))
        {
            std::cout << "Failed to get current serial parameters." << std::endl;
            return 3;
        }
        else
        {
            dcbSerialParameters.BaudRate = baudRate;
            dcbSerialParameters.ByteSize = 8;
            dcbSerialParameters.StopBits = ONESTOPBIT;
            dcbSerialParameters.Parity = NOPARITY;
            dcbSerialParameters.fDtrControl = DTR_CONTROL_ENABLE;

            if (!SetCommState(handler, &dcbSerialParameters))
            {
                std::cout << "ALERT: could not set Serial port parameters." << std::endl;
                return 4;
            }
            else
            {
                this->connected = true;
                PurgeComm(this->handler, PURGE_RXCLEAR | PURGE_TXCLEAR);
            }
        }
    }
    return 0;
}

int SerialPort::readSerialPort(char* buffer, unsigned int buf_size)
{
    DWORD bytesRead;
    unsigned int toRead = 0;

    ClearCommError(this->handler, &this->errors, &this->status);

    if (this->status.cbInQue > 0) 
    {
        if (this->status.cbInQue > buf_size) 
        {
            toRead = buf_size;
        }
        else
        {
            toRead = this->status.cbInQue;
        }
    }

    memset(buffer, 0, buf_size);

    if (ReadFile(this->handler, buffer, toRead, &bytesRead, NULL))
    {
        return bytesRead;
    }

    return 0;
}

bool SerialPort::writeSerialPort(char* buffer, unsigned int buf_size)
{
    DWORD bytesSend;

    if (!WriteFile(this->handler, (void*)buffer, buf_size, &bytesSend, 0)) 
    {
        ClearCommError(this->handler, &this->errors, &this->status);
        return false;
    }
    else
    {
        return true;
    }
}

bool SerialPort::isConnected()
{
    if (!ClearCommError(this->handler, &this->errors, &this->status))
    {
        this->connected = false;
    }

    return this->connected;
}

void SerialPort::closeSerial()
{
    CloseHandle(this->handler);
}






LedDeviceFinder::LedDeviceFinder(const char* sDeviceName)
{
    this->deviceName = sDeviceName;
}

LedDeviceFinder::~LedDeviceFinder()
{
}

bool LedDeviceFinder::FindConnectedDevice(std::string &data)
{
    bool bDeviceFound = false;
    char targetName[5000] = {'\0'};

    /* Check all available COM ports */
    for (unsigned int portCount = 0; portCount < 255; portCount++)
    {
        /* Get COM port name */
        std::string portNameTmp = "COM" + std::to_string(portCount);
        DWORD result = QueryDosDevice(portNameTmp.c_str(), targetName, 5000);

        if (result)
        {
            /* COM port found */
            if (strcmp(this->deviceName, targetName) == 0)
            {
                data = "\\\\.\\" + portNameTmp;
                bDeviceFound = true;
            }
        }
        else
        {
            /* Do nothing */
        }
    }

    return bDeviceFound;
}
