#include <windows.h>
#include <iostream>

using namespace std;

struct Actions
{
    const char* encrypt = "--encrypt";
    const char* decrypt = "--decrypt";
    const char* both    = "--both";
};

struct GeneralMessages
{
    string encMessage      = "\n   Encrypted text : ";
    string decMessage      = "\n   Decrypted text : ";
    string encUsagMessage  = "\n   Encrypt :         TMS.exe --encrypt \"simpletext\"";
    string decUsagMessage  = "\n   Decrypt :         TMS.exe --decrypt \"encryptedtext\"";
    string bothUsagMessage = "\n   Encrypt/Decrypt : TMS.exe --both    \"simpletext\"";
};

string encryptString(const string& smplText) 
{
    string encrypted = smplText;
    char al;

    for (char& cl : encrypted) 
    {
        al = cl;
        al ^= 5;
        al &= 0x1F;
        cl &= 0xE0;
        cl += al;
    }

    return encrypted;
}

string decryptString(const string& encText)
{
    string decrypted = encText;
    char al;

    for (char& cl : decrypted)
    {
        al = cl;
        cl &= 0xE0;
        al &= 0x1F;
        al ^= 5;
        cl += al;
    }

    return decrypted;
}

int main(int argc, char* argv[])
{
    bool actStatus = false;

    Actions action;
    GeneralMessages generalMessage;
  
    if (argc == 3)
    {
        if (strncmp(argv[1], action.encrypt,strlen(action.encrypt)) == 0)
        {
            cout << generalMessage.encMessage << encryptString(argv[2]) << "\n" << endl;
            actStatus = true;
        }
        else if (strncmp(argv[1], action.decrypt, strlen(action.decrypt)) == 0)
        {
            cout << generalMessage.decMessage << decryptString(argv[2]) << "\n" << endl;
            actStatus = true;
        }
        else if (strncmp(argv[1], action.both, strlen(action.both)) == 0)
        {
            string encStr = encryptString(argv[2]);
            string decStr = decryptString(encStr);
            cout << generalMessage.encMessage << encryptString(encStr);
            cout << generalMessage.decMessage << decryptString(decStr) << "\n" << endl;
            actStatus = true;
        }
        else
        {
            cout << generalMessage.encUsagMessage << generalMessage.decUsagMessage << generalMessage.bothUsagMessage << "\n" << endl;
            actStatus = false;
        }
    }
    else
    {
        cout << generalMessage.encUsagMessage << generalMessage.decUsagMessage << generalMessage.bothUsagMessage << "\n" << endl;
        actStatus = false;
    }


    getchar();
    return 0;
}
