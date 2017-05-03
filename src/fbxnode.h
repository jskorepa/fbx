#ifndef FBXNODE_H
#define FBXNODE_H

#include "fbxproperty.h"

namespace fbx {

class FBXNode
{
public:
    FBXNode();

    std::uint32_t read(std::ifstream &input, uint32_t start_offset);
    void print(std::string prefix="");
    bool isNull();

    std::vector<FBXNode> children;
    std::vector<FBXProperty> properties;
    std::string name;

};

} // namespace fbx

#endif // FBXNODE_H
