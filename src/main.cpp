#include <stdint.h>
#include <iostream>

#include "fbxdocument.h"

int main(int argc, char** argv)
{
    fbx::FBXDocument doc;
    doc.createBasicStructure();
    doc.print();

    return 0;
}
