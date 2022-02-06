#include "SerialPort.h"

SerialPort::SerialPort(unsigned long lBaudRate)
{
    this->m_handler = nullptr;
    this->m_connected = false;
    this->m_errors = 0;
    this->m_baudRate = lBaudRate;
    this->m_status = { 0 };

    /* Map supported devices to enum. */
    for (int a = DEVICE_ESP; a < DEVICE_COUNT; a++)
    {
        m_devices.insert({ DEVICE_ESP, supported_devices[a] });
    }
}

SerialPort::~SerialPort()
{
    if (this->m_connected) 
    {
        this->m_connected = false;
        CloseHandle(this->m_handler);
    }

    if (this->m_handler != nullptr)
    {
        this->m_handler = nullptr;
    }
}

unsigned int SerialPort::initSerialPort()
{
    /* Find all COM devices connected to the computer. */
    m_ports = FindConnectedDevices();

    if (!m_ports.empty())
    {
        /* Check if connected device is on the list of supported devices. */
        if (IsSupported(m_ports))
        {
            this->m_handler = CreateFileA(static_cast<LPCSTR>(m_portName.c_str()), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

            if (this->m_handler == INVALID_HANDLE_VALUE)
            {
                if (GetLastError() == ERROR_FILE_NOT_FOUND)
                {
                    std::cout << "ERROR: Handle was not attached.Reason : " + m_portName + "not available." << std::endl;
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

                if (!GetCommState(this->m_handler, &dcbSerialParameters))
                {
                    std::cout << "Failed to get current serial parameters." << std::endl;
                    return 3;
                }
                else
                {
                    dcbSerialParameters.BaudRate = m_baudRate;
                    dcbSerialParameters.ByteSize = 8;
                    dcbSerialParameters.StopBits = ONESTOPBIT;
                    dcbSerialParameters.Parity = NOPARITY;
                    dcbSerialParameters.fDtrControl = DTR_CONTROL_ENABLE;

                    if (!SetCommState(m_handler, &dcbSerialParameters))
                    {
                        std::cout << "ALERT: could not set Serial port parameters." << std::endl;
                        return 4;
                    }
                    else
                    {
                        this->m_connected = true;
                        PurgeComm(this->m_handler, PURGE_RXCLEAR | PURGE_TXCLEAR);
                    }
                }
            }
        }
        else
        {
            return 1;
        }
    }
    else
    {
        return 1;
    }

    
    return 0;
}

int SerialPort::readSerialPort(char* buffer, unsigned int buf_size)
{
    DWORD bytesRead;
    unsigned int toRead = 0;

    ClearCommError(this->m_handler, &this->m_errors, &this->m_status);

    if (this->m_status.cbInQue > 0) 
    {
        if (this->m_status.cbInQue > buf_size) 
        {
            toRead = buf_size;
        }
        else
        {
            toRead = this->m_status.cbInQue;
        }
    }

    memset(buffer, 0, buf_size);

    if (ReadFile(this->m_handler, buffer, toRead, &bytesRead, NULL))
    {
        return bytesRead;
    }

    return 0;
}

bool SerialPort::writeSerialPort(char* buffer, unsigned int buf_size)
{
    DWORD bytesSend;

    if (!WriteFile(this->m_handler, (void*)buffer, buf_size, &bytesSend, 0)) 
    {
        ClearCommError(this->m_handler, &this->m_errors, &this->m_status);
        return false;
    }
    else
    {
        return true;
    }
}

bool SerialPort::isConnected()
{
    if (!ClearCommError(this->m_handler, &this->m_errors, &this->m_status))
    {
        this->m_connected = false;
    }

    return this->m_connected;
}

std::map<std::string, std::string> SerialPort::FindConnectedDevices()
{
    std::map<std::string, std::string> devices;
    bool bDeviceFound = false;
    char targetName[5000] = {'\0'};

    /* Check all available COM ports */
    for (unsigned int portCount = 0; portCount < 255; portCount++)
    {
        /* Get COM port name */
        bDeviceFound = bool(QueryDosDevice(("COM" + std::to_string(portCount)).c_str(), targetName, 5000));

        if (bDeviceFound)
        {
            /* Add device to the vector */
            devices.insert({ std::string(targetName) ,("\\\\.\\COM" + std::to_string(portCount)) });
        }
        else
        {
            /* Do nothing */
        }
    }

    return devices;
}

bool SerialPort::IsSupported(std::map<std::string, std::string>& devices)
{
    for (auto device : devices)
    {
        for (auto supp_device : m_devices)
        {
            if (device.first == "\\Device\\" + supp_device.second)
            {
                m_portName = device.second;
                return true;
            }
        }
    }

    return false;
}
