#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdio>
#include <vector>     //for vector
#include <iterator>   //for std::istream_iterator and std::ostream_iterator
#include <algorithm>  //for std::copy

using namespace std;

string exeSig = "MZ";


struct DOS_Header
 {
// short is 2 bytes, long is 4 bytes
     char signature[2];
     short lastsize;
     short nblocks;
     short nreloc;
     short hdrsize;
     short minalloc;
     short maxalloc;
     void *ss;
     void *sp;
     short checksum;
     void *ip;
     void *cs;
     short relocpos;
     short noverlay;
     short reserved1[4];
     short oem_id;
     short oem_info;
     short reserved2[10];
     long  e_lfanew;
     unsigned int exOff;
 };

DOS_Header exeh;

//FONKSIYONLAR STRING DONMEMELI UNSIGNED CHAR DONMELI YA DA UNSIGNED INT DONMELI
string readUnBoundedBytes(int beginOff, int endOff, vector<unsigned char>& buf){
    string str(buf.begin() + beginOff, buf.begin() + beginOff + endOff);
    return str;
}

unsigned char* read2Bytes(DOS_Header exef, vector<unsigned char>& buf){
    //string str(buf.begin() + beginOff, buf.begin() + beginOff + 2);
    /*copy(buf.begin() + beginOff,
                  buf.begin() + beginOff + 2,
                  std::ostream_iterator<unsigned int>(cout << hex));*/

    vector<unsigned char> twoByteBuf(buf.begin() + exef.exOff, buf.begin() + + exef.exOff + 2);
    exef.exOff += 1;

    return reinterpret_cast<unsigned char*>(twoByteBuf.data());;
}

string read4Bytes(int beginOff, vector<unsigned char>& buf){
    string str(buf.begin() + beginOff, buf.begin() + beginOff + 4);
    return str;
}

string read8Bytes(int beginOff, vector<unsigned char>& buf){
    string str(buf.begin() + beginOff, buf.begin() + beginOff + 8);
    return str;
}

string read16Bytes(int beginOff, vector<unsigned char>& buf){
    string str(buf.begin() + beginOff, buf.begin() + beginOff + 16);
    return str;
}

string read32Bytes(int beginOff, vector<unsigned char>& buf){
    string str(buf.begin() + beginOff, buf.begin() + beginOff + 32);
    return str;
}


int readSignature(vector<unsigned char>& buf){

    std::string str(buf.begin(), buf.begin() + 2);
    if(str.compare(exeSig) == 1) return 1;

    return 0;
}

void readDosHeader(vector<unsigned char>& buf, DOS_Header exeh){

    //exeh.lastsize = boost::lexical_cast<short> ();
    std::stringstream ss;

    unsigned char * data = read2Bytes(exeh, buf);

    for(int i=0; i<2; ++i)
        ss << hex << (unsigned int)data[i];
    string mystr = ss.str();

    cout << mystr;
}

int main (int argc, char *argv[])
{
    string path = argv[1];

    ifstream fd(path, ios::in|ios::binary);

    std::istream_iterator<unsigned char> begin(fd), end;
    std::vector<unsigned char> buffer(begin, end);

    if(readSignature(buffer)) { readDosHeader(buffer, exeh); }



    /*std::copy(buffer.begin(),
              buffer.end(),
              std::ostream_iterator<unsigned int>(std::cout << hex, ","));*/

}


