#include "fbxproperty.h"
#include "fbxutil.h"
#include <functional>
#include <zlib.h>

using std::cout;
using std::endl;

namespace fbx {

namespace { // helpers for reading properties
    FBXPropertyValue readPrimitiveValue(Reader &reader, char type)
    {
        FBXPropertyValue value;
        if(type == 'Y') { // 2 byte signed integer
            value.i16 = reader.readInt16();
        } else if(type == 'C' || type == 'B') { // 1 bit boolean (1: true, 0: false) encoded as the LSB of a 1 Byte value.
            value.boolean = reader.readUint8() != 0;
        } else if(type == 'I') { // 4 byte signed Integer
            value.i32 = reader.readInt32();
        } else if(type == 'F') { // 4 byte single-precision IEEE 754 number
            value.f32 = reader.readFloat();
        } else if(type == 'D') { // 8 byte double-precision IEEE 754 number
            value.f64 = reader.readDouble();
        } else if(type == 'L') { // 8 byte signed Integer
            value.i64 = reader.readUint64();
        } else {
            throw std::string("Unsupported property type ")+std::to_string(type);
        }
        return value;
    }

    class STRMAutoCloser
    {
    public:
        z_stream *strm;
        STRMAutoCloser(z_stream& _strm):strm(&_strm) {}

        ~STRMAutoCloser() {
            (void)inflateEnd(strm);
        }
    };

    class BufferAutoFree
    {
    public:
        char *buffer;
        BufferAutoFree(char *buf):buffer(buf) {}

        ~BufferAutoFree() {
            free(buffer);
        }
    };

    int inf(std::ifstream &source, char *out, uint32_t compressedLength)
    {
        uint32_t bufsize = compressedLength;

        int ret;
        z_stream strm;
        char in[bufsize];

        /* allocate inflate state */
        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;
        strm.avail_in = 0;
        strm.next_in = Z_NULL;
        ret = inflateInit(&strm);
        if (ret != Z_OK)
            return ret;
        STRMAutoCloser autocloser(strm);

        /* decompress until deflate stream ends or end of file */
        do {
            source.read(in, bufsize);
            strm.avail_in = bufsize;
            if (strm.avail_in == 0)
                break;
            strm.next_in = (unsigned char*)in;

            /* run inflate() on input until output buffer not full */
            do {
                strm.avail_out = bufsize;
                strm.next_out = (unsigned char*) out;
                ret = inflate(&strm, Z_NO_FLUSH);
                if(ret == Z_STREAM_ERROR) throw std::string("ZLiB ERROR");
                switch (ret) {
                case Z_NEED_DICT:
                    ret = Z_DATA_ERROR;
                    /* fallthrough */
                case Z_DATA_ERROR:
                case Z_MEM_ERROR:
                    (void)inflateEnd(&strm);
                    return ret;
                }
            } while (strm.avail_out == 0);

            /* done when inflate() says it's done */
        } while (ret != Z_STREAM_END);

        /* clean up and return */

        return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
    }
}

FBXProperty::FBXProperty(std::ifstream &input)
{
    Reader reader(&input);
    type = reader.readUint8();
    if(type == 'S' || type == 'R') {
        uint32_t length = reader.readUint32();
        for(uint32_t i = 0; i < length; i++){
            uint8_t v = reader.readUint8();
            raw.push_back(v);
        }
    } else if(type < 'Z') { // primitive types
        value = readPrimitiveValue(reader, type);
    } else {
        uint32_t arrayLength = reader.readUint32();
        uint32_t encoding = reader.readUint32();
        uint32_t compressedLength = reader.readUint32();
        if(encoding) {
            //reader.readString(compressedLength);
            char *decompressed = (char*) malloc(compressedLength*100);
            if(decompressed == NULL) throw std::string("Malloc failed");
            BufferAutoFree bufferAutoFree(decompressed);
            int infRet = inf(input, decompressed, compressedLength);
            Reader r(decompressed);

            for(uint32_t i = 0; i < arrayLength; i++) {
                values.push_back(readPrimitiveValue(r, type - ('a'-'A')));
            }

            if(infRet) throw std::string("Zlib inflate failed");
        } else {
            for(uint32_t i = 0; i < arrayLength; i++) {
                values.push_back(readPrimitiveValue(reader, type - ('a'-'A')));
            }
        }
    }
}

// primitive values
FBXProperty::FBXProperty(int16_t a) { type = 'Y'; value.i16 = a; }
FBXProperty::FBXProperty(bool a) { type = 'C'; value.boolean = a; }
FBXProperty::FBXProperty(int32_t a) { type = 'I'; value.i32 = a; }
FBXProperty::FBXProperty(float a) { type = 'F'; value.f32 = a; }
FBXProperty::FBXProperty(double a) { type = 'D'; value.f64 = a; }
FBXProperty::FBXProperty(int64_t a) { type = 'L'; value.i64 = a; }
// arrays
FBXProperty::FBXProperty(const std::vector<bool> a) : type('b'), values(a.size()) {
    for(auto el : a) {
        FBXPropertyValue v;
        v.boolean = el;
        values.push_back(v);
    }
}
FBXProperty::FBXProperty(const std::vector<int32_t> a) : type('i'), values(a.size()) {
    for(auto el : a) {
        FBXPropertyValue v;
        v.i32 = el;
        values.push_back(v);
    }
}
FBXProperty::FBXProperty(const std::vector<float> a) : type('f'), values(a.size()) {
    for(auto el : a) {
        FBXPropertyValue v;
        v.f32 = el;
        values.push_back(v);
    }
}
FBXProperty::FBXProperty(const std::vector<double> a) : type('d'), values(a.size()) {
    for(auto el : a) {
        FBXPropertyValue v;
        v.f64 = el;
        values.push_back(v);
    }
}
FBXProperty::FBXProperty(const std::vector<int64_t> a) : type('l'), values(a.size()) {
    for(auto el : a) {
        FBXPropertyValue v;
        v.i64 = el;
        values.push_back(v);
    }
}
// raw / string
FBXProperty::FBXProperty(const std::vector<uint8_t> a, uint8_t type): raw(a) {
    if(type != 'R' && type != 'S') {
        throw std::string("Bad argument to FBXProperty constructor");
    }
}

void FBXProperty::print(std::string prefix)
{
    cout << prefix << type << ": ";
    if(type == 'Y') cout << value.i16;
    else if(type == 'C') cout << value.boolean;
    else if(type == 'I') cout << value.i32;
    else if(type == 'F') cout << value.f32;
    else if(type == 'D') cout << value.f64;
    else if(type == 'L') cout << value.i64;
    else if(type == 'R') {
        for(char c : raw) {
            cout << std::to_string(c) << " ";
        }
    } else if(type == 'S') {
        cout << "(" << raw.size() << ") ";
        for(char c : raw) {
            cout << c;
        }
    } else {
        for(auto e : values) {
            if(type == 'f') cout << e.f32;
            else if(type == 'd') cout << e.f64;
            else if(type == 'l') cout << e.i64;
            else if(type == 'i') cout << e.i32;
            else if(type == 'b') cout << e.boolean;
        }
    }
    cout << "\n";
}

} // namespace fbx
