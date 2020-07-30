#ifndef FBXDOCUMENT_H
#define FBXDOCUMENT_H

#include "fbxnode.h"
#include <fstream>
using namespace std;

namespace fbx {

class FBXDocument
{
public:
    FBXDocument();
    void read(std::ifstream &input);
    void read(std::string fname);
    void write(std::string fname);
    void write(std::ofstream &output);

    void createBasicStructure();

    std::vector<FBXNode> nodes;

    std::uint32_t getVersion();
    void print();
	void log(ofstream &output);

private:
    std::uint32_t version;
};

} // namespace fbx

#endif // FBXDOCUMENT_H
