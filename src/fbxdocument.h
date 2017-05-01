#ifndef FBXDOCUMENT_H
#define FBXDOCUMENT_H

#include "fbxnode.h"

namespace fbx {

class FBXDocument
{
public:
    FBXDocument();
    void read(std::ifstream &input);
    void read(std::string &fname);

    std::vector<FBXNode> nodes;

private:
    std::uint32_t version;
};

} // namespace fbx

#endif // FBXDOCUMENT_H
