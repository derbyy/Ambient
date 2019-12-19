#include "SerialPort.h"
#include <chrono>
#include <vector>
#include <fstream>

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


bool SaveImage(const std::string& szPathName, void* lpBits, int w, int h) 
{
    // Create a new file for writing
    std::ofstream pFile(szPathName, std::ios_base::binary);
    if (!pFile.is_open()) {
        return false;
    }

    BITMAPINFOHEADER bmih;
    bmih.biSize = sizeof(BITMAPINFOHEADER);
    bmih.biWidth = w;
    bmih.biHeight = h;
    bmih.biPlanes = 1;
    bmih.biBitCount = 24;
    bmih.biCompression = BI_RGB;
    bmih.biSizeImage = w * h * 3;

    BITMAPFILEHEADER bmfh;
    int nBitsOffset = sizeof(BITMAPFILEHEADER) + bmih.biSize;
    LONG lImageSize = bmih.biSizeImage;
    LONG lFileSize = nBitsOffset + lImageSize;
    bmfh.bfType = 'B' + ('M' << 8);
    bmfh.bfOffBits = nBitsOffset;
    bmfh.bfSize = lFileSize;
    bmfh.bfReserved1 = bmfh.bfReserved2 = 0;

    // Write the bitmap file header
    pFile.write((const char*)&bmfh, sizeof(BITMAPFILEHEADER));
    UINT nWrittenFileHeaderSize = pFile.tellp();

    // And then the bitmap info header
    pFile.write((const char*)&bmih, sizeof(BITMAPINFOHEADER));
    UINT nWrittenInfoHeaderSize = pFile.tellp();

    // Finally, write the image data itself
    //-- the data represents our drawing
    pFile.write((const char*)lpBits, bmih.biSizeImage);
    UINT nWrittenDIBDataSize = pFile.tellp();
    pFile.close();

    return true;
}

void imageProcess()
{
    BITMAP bmpScreen;
    byte* bitPointer;
    unsigned long red = 0, green = 0, blue = 0, alpha = 0, size = 0;

    /* Retrieve the handle to a display device context for the client area of the window. */
    HDC hScreenDC = GetDC(NULL);

    /* Create a compatible DC which is used in a BitBlt from the window DC */
    HDC hMemoryDC = CreateCompatibleDC(hScreenDC);

    /* Set number of pixel to x and y axis */
    int horizontal = 120;
    int vertical = 120;

    BITMAPINFO bitmap;
    bitmap.bmiHeader.biSize = sizeof(bitmap.bmiHeader);
    bitmap.bmiHeader.biWidth = horizontal;
    bitmap.bmiHeader.biHeight = vertical;
    bitmap.bmiHeader.biPlanes = 1;
    bitmap.bmiHeader.biBitCount = 24;
    bitmap.bmiHeader.biCompression = BI_RGB;
    bitmap.bmiHeader.biSizeImage = 0;
    bitmap.bmiHeader.biClrUsed = 0;
    bitmap.bmiHeader.biClrImportant = 0;

    size = unsigned long(vertical * ((horizontal * 3 + 3) & 0xFFFFFFFC));

    HBITMAP hBitMap = CreateDIBSection(hMemoryDC, &bitmap, DIB_RGB_COLORS, (void**)(&bitPointer), NULL, NULL);
    SelectObject(hMemoryDC, hBitMap);

    //while (true)
    //{
        Timer timer("inside while");
        BitBlt(hMemoryDC, 0, 0, horizontal, vertical, hScreenDC, 120, 0, SRCCOPY);

        SaveImage("obr.bmp", bitPointer, horizontal, vertical);

        for (int i = 0; i < size; i += 4)
        {
            red += unsigned long(bitPointer[i]);
            green += unsigned long(bitPointer[i + 1]);
            blue += unsigned long(bitPointer[i + 2]);
        }

        red = red / (horizontal * vertical);
        green = green / (horizontal * vertical);
        blue = blue / (horizontal * vertical);
    //}
}












int main()
{
    //for (int i = 0; i < 100; i++)
    //{
    //    {
            //Timer timer("imageProcess");
            imageProcess();
    //    }
    //}

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
