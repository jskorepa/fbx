#ifndef FBXPROPERTY_H
#define FBXPROPERTY_H

#include <memory>
#include <iostream>
#include <vector>

namespace fbx {

// WARNING: (copied from fbxutil.h)
// this assumes that float is 32bit and double is 64bit
// both conforming to IEEE 754, it does not assume endianness
// it also assumes that signed integers are two's complement
union FBXPropertyValue {
    int16_t i16;  // Y
    bool boolean; // C, b
    int32_t i32;  // I, i
    float f32;    // F, f
    double f64;   // D, d
    int64_t i64;  // L, l
};

class FBXProperty
{
public:
    FBXProperty(std::ifstream &input);

    void print(std::string prefix="");

    bool is_array();
private:
    char type;
    FBXPropertyValue value;
    std::vector<uint8_t> raw;
    std::vector<FBXPropertyValue> values;
};

} // namespace fbx

#endif // FBXPROPERTY_H
