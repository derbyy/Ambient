#include "SerialPort.h"
#include <d3d9.h>



#include <chrono>

class Timer
{
public:
    Timer(std::string funcName)
    {
        m_StartTimepoint = std::chrono::high_resolution_clock::now();
        m_FunctionName = funcName;
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
        std::cout << "Function " << m_FunctionName << " takes : " << duration << " us" << std::endl;
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
    std::string m_FunctionName;
};


void CaptureImage()
{
    BITMAP bmpScreen;

    /* Retrieve the handle to a display device context for the client area of the window. */
    HDC hdcScreen = GetDC(NULL);

    /* Create a compatible DC which is used in a BitBlt from the window DC */
    HDC hdcMemDC = CreateCompatibleDC(hdcScreen);

    int width = 200;// GetDeviceCaps(hdcScreen, HORZRES);
    int height = 200;// GetDeviceCaps(hdcScreen, VERTRES);

        
    HBITMAP hbmScreen = CreateCompatibleBitmap(hdcScreen, width, height);

    SelectObject(hdcMemDC, hbmScreen);

    BitBlt(hdcMemDC, 0, 0, width, height, hdcScreen, 0, 0, SRCCOPY);

    GetObject(hbmScreen, sizeof(BITMAP), &bmpScreen);

    DeleteDC(hdcMemDC);
    DeleteDC(hdcScreen);
}

void test(HDC screen, HDC mem, int width, int height)
{
    BITMAP bmpScreen;

    HBITMAP hbmScreen = CreateCompatibleBitmap(screen, width, height);
    SelectObject(mem, hbmScreen);

    BitBlt(mem, 0, 0, width, height, screen, 0, 0, SRCCOPY);

    GetObject(hbmScreen, sizeof(BITMAP), &bmpScreen);
}





int main()
{


    HDC hdcScreen = GetDC(NULL);
    HDC hdcMemDC = CreateCompatibleDC(hdcScreen);

    for (int i = 0; i < 10; i++)
    {
        {
            //Timer timer("CaptureImage");
            CaptureImage();
        }
    }









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
