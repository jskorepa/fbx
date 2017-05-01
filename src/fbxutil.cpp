#include "fbxutil.h"

namespace fbx {

namespace {
    bool isLittleEndian()
    {
        uint16_t number = 0x1;
        char *numPtr = (char*)&number;
        return (numPtr[0] == 1);
    }
}

uint8_t Reader::readUint8()
{
    return getc();
}

int8_t Reader::readInt8()
{
    return getc();
}

uint16_t Reader::readUint16()
{
    uint16_t i;
    char *c = (char *)(&i);
    if(isLittleEndian()) {
        read(c, 2);
    } else {
        c[1] = getc();
        c[0] = getc();
    }
    return i;
}

int16_t Reader::readInt16()
{
    int16_t i;
    char *c = (char *)(&i);
    if(isLittleEndian()) {
        read(c, 2);
    } else {
        c[1] = getc();
        c[0] = getc();
    }
    return i;
}

uint32_t Reader::readUint32()
{
    uint32_t i;
    char *c = (char *)(&i);
    if(isLittleEndian()) {
        read(c, 4);
    } else {
        c[3] = getc();
        c[2] = getc();
        c[1] = getc();
        c[0] = getc();
    }
    return i;
}

int32_t Reader::readInt32()
{
    int32_t i;
    char *c = (char *)(&i);
    if(isLittleEndian()) {
        read(c, 4);
    } else {
        c[3] = getc();
        c[2] = getc();
        c[1] = getc();
        c[0] = getc();
    }
    return i;
}

uint64_t Reader::readUint64()
{
    uint64_t i;
    char *c = (char *)(&i);
    if(isLittleEndian()) {
        read(c, 8);
    } else {
        c[7] = getc();
        c[6] = getc();
        c[5] = getc();
        c[4] = getc();
        c[3] = getc();
        c[2] = getc();
        c[1] = getc();
        c[0] = getc();
    }
    return i;
}


std::string Reader::readString(uint32_t length)
{
    char buffer[length + 1];
    buffer[length] = 0;
    if(length) read(buffer, length);
    return std::string(buffer);
}

float Reader::readFloat()
{
    float f;
    char *c = (char *)(&f);
    if(isLittleEndian()) {
        read(c, 4);
    } else {
        c[3] = getc();
        c[2] = getc();
        c[1] = getc();
        c[0] = getc();
    }
    return f;
}

double Reader::readDouble()
{
    double f;
    char *c = (char *)(&f);
    if(isLittleEndian()) {
        read(c, 8);
    } else {
        c[7] = getc();
        c[6] = getc();
        c[5] = getc();
        c[4] = getc();
        c[3] = getc();
        c[2] = getc();
        c[1] = getc();
        c[0] = getc();
    }
    return f;
}

Reader::Reader(std::ifstream *input)
    :ifstream(input),buffer(NULL),i(0)
{}

Reader::Reader(char *input)
    :ifstream(NULL),buffer(input),i(0)
{}

uint8_t Reader::getc()
{
    uint8_t tmp;
    if(ifstream != NULL) (*ifstream) >> tmp;
    else tmp = buffer[i++];
    return tmp;
}

void Reader::read(char *s, uint32_t n)
{
    if(ifstream != NULL) ifstream->read(s, n);
    else for(uint32_t a = 0; a < n; a++) {
        s[a] = buffer[i++];
    }
}

} // namespace fbx
