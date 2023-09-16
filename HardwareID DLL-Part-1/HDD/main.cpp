#include <iostream>
#include <windows.h>
#include "md516.h"

using namespace std;

#define SERIAL_NUMBER_OFFSET       0x24
#define SERIAL_NUMBER_LENGTH       0x11
#define HARDWARE_ID_LENGTH         0x14
#define MAXIMUM_HARDWARE_ID_LENGTH 0x18
#define MAXIMUM_DUMMY_SIZE         0x63
#define CUSTOM_BUFFER_SIZE         0x400

struct GeneralMessages
{
    string errCode = "Error code is : ";
    const wchar_t* device = L"\\\\.\\PhysicalDrive0";
};

void reorderSerialNumber(char* input, char* output, size_t length) 
{
    for (size_t i = 0; i < length; i += 2) 
    {
        if (i + 1 < length) 
        {
            output[i] = input[i + 1];
            output[i + 1] = input[i];
        }
        else 
        {
            output[i] = input[i];
        }
    }
}

void cleanSerialNumber(char* input, wchar_t* output, size_t length)
{
    size_t j = 0; 

    for (size_t i = 0; i < length; ++i) 
    {
        if (input[i] != 0x20) 
        {
            output[j++] = input[i];
        }
    }
}

void createHardwareID(wchar_t* input, char* output)
{
    CMD5Ex MP;
    MP.Init();
    CString md5Hash;
    MP.Md5(md5Hash, input, MD516);

    size_t oldLen = wcslen(md5Hash);
    char upperMd5Hash[SERIAL_NUMBER_LENGTH];
    SecureZeroMemory(upperMd5Hash, sizeof(upperMd5Hash));

    for (size_t i = 0; i < oldLen; i++) 
    {
        upperMd5Hash[i] = towupper(md5Hash[i]);
    }

    int chunkSize = 4;
    char delimiter[] = "-";
    int newLen = oldLen + (oldLen / chunkSize);
    char finalHash[HARDWARE_ID_LENGTH];
    SecureZeroMemory(finalHash, sizeof(finalHash));

    int j = 0;

    for (int i = 0; i < oldLen; i++)
    {
        output[j++] = upperMd5Hash[i];

        if ((i + 1) % chunkSize == 0 && (i + 1) != oldLen) 
        {
            output[j++] = delimiter[0];
        }
    }
}

int getHDD()
{
    GeneralMessages errgeneralMessage;

    HANDLE hPhysicalDrive = CreateFile(
        errgeneralMessage.device,
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_READONLY,
        NULL,
        NULL
    );

    if (hPhysicalDrive == INVALID_HANDLE_VALUE) 
    {
        cerr << errgeneralMessage.errCode << GetLastError() << endl;
        return 1;
    }


    DWORD bufferSize = CUSTOM_BUFFER_SIZE;
    BYTE* buffer = new BYTE[bufferSize];
    DWORD bytesReturned;

    SENDCMDINPARAMS inParams = { 0 };
    inParams.irDriveRegs.bCommandReg = ID_CMD;
    inParams.cBufferSize = bufferSize;

    if (!DeviceIoControl(hPhysicalDrive, SMART_RCV_DRIVE_DATA, &inParams, sizeof(inParams), buffer,
        bufferSize, &bytesReturned, NULL))
    {
        cerr << errgeneralMessage.errCode << GetLastError() << endl;
        delete[] buffer;
        CloseHandle(hPhysicalDrive);
        return 1;

    }

    char serialNumber[MAXIMUM_HARDWARE_ID_LENGTH];
    SecureZeroMemory(serialNumber, sizeof(serialNumber));
    memcpy_s(serialNumber, sizeof(serialNumber), &buffer[SERIAL_NUMBER_OFFSET], sizeof(serialNumber));

    char reorderedSerialNumber[sizeof(serialNumber)];
    SecureZeroMemory(reorderedSerialNumber, sizeof(reorderedSerialNumber));
    reorderSerialNumber(serialNumber, reorderedSerialNumber, sizeof(serialNumber));

    wchar_t cleanedSerialNumber[sizeof(serialNumber)];
    SecureZeroMemory(cleanedSerialNumber, sizeof(cleanedSerialNumber));
    cleanSerialNumber(reorderedSerialNumber, cleanedSerialNumber, sizeof(serialNumber));

    char hardwareID[HARDWARE_ID_LENGTH];
    SecureZeroMemory(hardwareID, sizeof(hardwareID));
    createHardwareID(cleanedSerialNumber, hardwareID);

    delete[] buffer;
    
    CloseHandle(hPhysicalDrive);

    return 0;
}

int main() 
{
    getHDD();
    return 0;
}
