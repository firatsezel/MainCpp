#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdio>

using namespace std;

string readFile(string path)
{
    stringstream ss;

    char magic[16];
    ifstream fd(path, ios::in|ios::binary);

    fd.read(magic,16);

    char a[16];
    sprintf(a, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x" ,
            (unsigned char)magic[0], (unsigned char)magic[1], (unsigned char)magic[2],
            (unsigned char)magic[3], (unsigned char)magic[4], (unsigned char)magic[5], (unsigned char)magic[6],
            (unsigned char)magic[7], (unsigned char)magic[8], (unsigned char)magic[9], (unsigned char)magic[10],
            (unsigned char)magic[11], (unsigned char)magic[12],(unsigned char)magic[13],(unsigned char)magic[14]
            ,(unsigned char)magic[15]);

    /*for(int i = 0; i < 4; ++i){
        ss << hex << (unsigned char)magic[i];
    }*/

    return a;
}

