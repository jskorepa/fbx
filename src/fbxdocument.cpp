#include "fbxdocument.h"
#include "fbxutil.h"

using std::string;
using std::cout;
using std::endl;
using std::ifstream;
using std::uint32_t;
using std::uint8_t;

namespace fbx {
FBXDocument::FBXDocument()
{
    version = 7400;
}

void FBXDocument::read(string &fname)
{
    ifstream file;

    // buffer
    int bufferSize = 1 << 16;
    char buffer[bufferSize];
    file.rdbuf()->pubsetbuf(buffer, bufferSize);

    file.open(fname, std::ios::in | std::ios::binary);
    if (file.is_open()) {
        read(file);
    } else {
        throw "Cannot read from file: \"" + fname + "\"";
    }
    file.close();
}

bool checkMagic(Reader &reader)
{
    string magic("Kaydara FBX Binary  ");
    for(char c : magic) {
        if(reader.readUint8() != c) return false;
    }
    if(reader.readUint8() != 0x00) return false;
    if(reader.readUint8() != 0x1A) return false;
    if(reader.readUint8() != 0x00) return false;
    return true;
}

void FBXDocument::read(std::ifstream &input)
{
    Reader reader(&input);
    input >> std::noskipws;
    if(!checkMagic(reader)) throw std::string("Not a FBX file");

    uint32_t version = reader.readUint32();
    cout << "Version: " << version << endl;

    uint32_t maxVersion = 7400;
    if(version > maxVersion) throw "Unsupported FBX version "+std::to_string(version)
                            + " latest supported version is "+std::to_string(maxVersion);

    uint32_t start_offset = 27; // magic: 21+2, version: 4
    do{
        FBXNode node;
        start_offset += node.read(input, start_offset);
        cout << "node: \"" << node.name << "\"\n";
        nodes.push_back(node);
    } while(!nodes.at(nodes.size()-1).isNull());
}
} // namespace fbx
