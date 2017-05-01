#ifndef FBXUTIL_H
#define FBXUTIL_H

#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <iostream>

namespace fbx {
    // WARNING:
    // this assumes that float is 32bit and double is 64bit
    // both conforming to IEEE 754, it does not assume endianness
    // it also assumes that signed integers are two's complement
    class Reader {
    public:
        Reader(std::ifstream *input);
        Reader(char *input);

        std::uint8_t readUint8();
        std::int8_t readInt8();
        std::uint16_t readUint16();
        std::int16_t readInt16();
        std::uint32_t readUint32();
        std::int32_t readInt32();
        std::uint64_t readUint64();
        std::string readString(uint32_t length);
        float readFloat();
        double readDouble();
    private:
        void read(char*, uint32_t);
        uint8_t getc();
        std::ifstream *ifstream;
        char *buffer;
        uint32_t i;
    };
}

#endif // FBXUTIL_H
