#include "fbxnode.h"

#include "fbxutil.h"
using std::string;
using std::cout;
using std::endl;
using std::ifstream;
using std::uint32_t;
using std::uint8_t;

namespace fbx {

FBXNode::FBXNode()
{
}

uint32_t FBXNode::read(std::ifstream &input, uint32_t start_offset)
{
    Reader reader(&input);
    uint32_t bytes = 0;

    uint32_t endOffset = reader.readUint32();
    uint32_t numProperties = reader.readUint32();
    uint32_t propertyListLength = reader.readUint32();
    uint8_t nameLength = reader.readUint8();
    name = reader.readString(nameLength);
    bytes += 13 + nameLength;

    for(uint32_t i = 0; i < numProperties; i++) {
        properties.push_back(FBXProperty(input));
    }
    bytes += propertyListLength;

    while(start_offset + bytes < endOffset) {
        FBXNode child;
        bytes += child.read(input, start_offset + bytes);
        children.push_back(std::move(child));
    }
    return bytes;
}

void FBXNode::print(std::string prefix)
{
    cout << prefix << "NODE " << name << "\n";
    if(properties.size() > 0) {
        cout << prefix << "Properties\n";
        for(FBXProperty prop : properties) {
            prop.print(prefix+"  ");
        }
    }
    if(children.size() > 0) {
        cout << prefix << "Children\n";
        for(FBXNode node : children) {
            node.print(prefix+"  ");
        }
    }
}

bool FBXNode::isNull()
{
    return children.size() == 0
            && properties.size() == 0
            && name.length() == 0;
}

} // namespace fbx
