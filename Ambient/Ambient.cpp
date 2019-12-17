#include "SerialPort.h"
#include <string>
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <string.h>
#include <psapi.h>
#include <strsafe.h>

#define BUFSIZE 512


bool SelectComPort() //added function to find the present serial 
{
    char lpTargetPath[10000]; // buffer to store the path of the COMPORTS
    bool gotPort = false; // in case the port is not found

    for (int i = 0; i < 255; i++) // checking ports from COM0 to COM255
    {
        std::string str = "COM" + std::to_string(i); // converting to COM0, COM1, COM2
        LPCWSTR tst = LPCWSTR(str.c_str());

        DWORD test = QueryDosDevice(tst, LPWSTR(lpTargetPath), 5000);

        // Test the return value and error if any
        if (test != 0) //QueryDosDevice returns zero if it didn't find an object
        {
            std::cout << str << ": " << lpTargetPath << std::endl;
            gotPort = true;
        }

        if (::GetLastError() == ERROR_INSUFFICIENT_BUFFER)
        {
        }
    }

    return gotPort;
}

int main()
{
    unsigned int err = 0;
    const char* comPortName = "\\\\.\\COM5";

    char buffer[6] = "test\n";
    char output[6] = {'\0'};

    std::string portName = "\\\\.\\COM5";
   
    SelectComPort();
    //SerialPort* port = new SerialPort(comPortName, 115200);

    //err = port->initSerialPort();

    //err = port->writeSerialPort(buffer, strlen(buffer));

    //err = port->readSerialPort(output, sizeof(output) / sizeof(char));

    //delete port;
}
