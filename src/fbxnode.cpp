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

FBXNode::FBXNode(std::string name):name(name) {}

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
        if(child.isNull()) break;
        children.push_back(std::move(child));
    }
    return bytes;
}

void FBXNode::print(std::string prefix)
{
    cout << prefix << "{ \"name\": \"" << name << "\",\n";
    if(properties.size() > 0) {
        cout << prefix << "  \"properties\": [\n";
        bool hasPrev = false;
        for(FBXProperty prop : properties) {
            if(hasPrev) cout << ",\n";
            cout << prefix << "    { \"type\": \"" << prop.getType() << "\", \"value\": " << prop.to_string() << " }";
            hasPrev = true;
        }
        cout << "\n";
        cout << prefix << "  ]" << (children.size() > 0 ? ",\n" : "\n");

    }

    if(children.size() > 0) {
        cout << prefix << "  \"children\": [\n";
        bool hasPrev = false;
        for(FBXNode node : children) {
            if(hasPrev) cout << ",\n";
            node.print(prefix+"    ");
            hasPrev = true;
        }
        cout << "\n";
        cout << prefix << "  ]\n";
    }

    cout << prefix << "}";

}

bool FBXNode::isNull()
{
    return children.size() == 0
            && properties.size() == 0
            && name.length() == 0;
}

// primitive values
void FBXNode::addProperty(int16_t v) { properties.push_back(FBXProperty(v)); }
void FBXNode::addProperty(bool v) { properties.push_back(FBXProperty(v)); }
void FBXNode::addProperty(int32_t v) { properties.push_back(FBXProperty(v)); }
void FBXNode::addProperty(float v) { properties.push_back(FBXProperty(v)); }
void FBXNode::addProperty(double v) { properties.push_back(FBXProperty(v)); }
void FBXNode::addProperty(int64_t v) { properties.push_back(FBXProperty(v)); }
// arrays
void FBXNode::addProperty(const std::vector<bool> v) { properties.push_back(FBXProperty(v)); }
void FBXNode::addProperty(const std::vector<int32_t> v) { properties.push_back(FBXProperty(v)); }
void FBXNode::addProperty(const std::vector<float> v) { properties.push_back(FBXProperty(v)); }
void FBXNode::addProperty(const std::vector<double> v) { properties.push_back(FBXProperty(v)); }
void FBXNode::addProperty(const std::vector<int64_t> v) { properties.push_back(FBXProperty(v)); }
// raw / string
void FBXNode::addProperty(const std::vector<uint8_t> v, uint8_t type) { properties.push_back(FBXProperty(v, type)); }
void FBXNode::addProperty(const std::string v) { properties.push_back(FBXProperty(v)); }
void FBXNode::addProperty(const char *v) { properties.push_back(FBXProperty(v)); }


void FBXNode::addPropertyNode(const std::string name, int16_t v) { FBXNode n(name); n.addProperty(v); children.push_back(n); }
void FBXNode::addPropertyNode(const std::string name, bool v) { FBXNode n(name); n.addProperty(v); children.push_back(n); }
void FBXNode::addPropertyNode(const std::string name, int32_t v) { FBXNode n(name); n.addProperty(v); children.push_back(n); }
void FBXNode::addPropertyNode(const std::string name, float v) { FBXNode n(name); n.addProperty(v); children.push_back(n); }
void FBXNode::addPropertyNode(const std::string name, double v) { FBXNode n(name); n.addProperty(v); children.push_back(n); }
void FBXNode::addPropertyNode(const std::string name, int64_t v) { FBXNode n(name); n.addProperty(v); children.push_back(n); }
void FBXNode::addPropertyNode(const std::string name, const std::vector<bool> v) { FBXNode n(name); n.addProperty(v); children.push_back(n); }
void FBXNode::addPropertyNode(const std::string name, const std::vector<int32_t> v) { FBXNode n(name); n.addProperty(v); children.push_back(n); }
void FBXNode::addPropertyNode(const std::string name, const std::vector<float> v) { FBXNode n(name); n.addProperty(v); children.push_back(n); }
void FBXNode::addPropertyNode(const std::string name, const std::vector<double> v) { FBXNode n(name); n.addProperty(v); children.push_back(n); }
void FBXNode::addPropertyNode(const std::string name, const std::vector<int64_t> v) { FBXNode n(name); n.addProperty(v); children.push_back(n); }
void FBXNode::addPropertyNode(const std::string name, const std::vector<uint8_t> v, uint8_t type) { FBXNode n(name); n.addProperty(v, type); children.push_back(n); }
void FBXNode::addPropertyNode(const std::string name, const std::string v) { FBXNode n(name); n.addProperty(v); children.push_back(n); }
void FBXNode::addPropertyNode(const std::string name, const char *v) { FBXNode n(name); n.addProperty(v); children.push_back(n); }

} // namespace fbx
