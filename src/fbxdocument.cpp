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

void FBXDocument::read(string fname)
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

    uint32_t maxVersion = 7400;
    if(version > maxVersion) throw "Unsupported FBX version "+std::to_string(version)
                            + " latest supported version is "+std::to_string(maxVersion);

    uint32_t start_offset = 27; // magic: 21+2, version: 4
    do{
        FBXNode node;
        start_offset += node.read(input, start_offset);
        if(node.isNull()) break;
        nodes.push_back(node);
    } while(true);
}

void FBXDocument::createBasicStructure()
{
    FBXNode headerExtension("FBXHeaderExtension");
    headerExtension.addPropertyNode("FBXHeaderVersion", (int32_t) 1003);
    headerExtension.addPropertyNode("FBXVersion", (int32_t) getVersion());
    headerExtension.addPropertyNode("EncryptionType", (int32_t) 0);
    {
        FBXNode creationTimeStamp("CreationTimeStamp");
        creationTimeStamp.addPropertyNode("Version", (int32_t) 1000);
        creationTimeStamp.addPropertyNode("Year", (int32_t) 2017);
        creationTimeStamp.addPropertyNode("Month", (int32_t) 5);
        creationTimeStamp.addPropertyNode("Day", (int32_t) 2);
        creationTimeStamp.addPropertyNode("Hour", (int32_t) 14);
        creationTimeStamp.addPropertyNode("Minute", (int32_t) 11);
        creationTimeStamp.addPropertyNode("Second", (int32_t) 46);
        creationTimeStamp.addPropertyNode("Millisecond", (int32_t) 917);
        headerExtension.children.push_back(creationTimeStamp);
    }
    headerExtension.addPropertyNode("Creator", "Blender (stable FBX IO) - 2.78 (sub 0) - 3.7.7");
    {
        FBXNode sceneInfo("SceneInfo");
        sceneInfo.addProperty(std::vector<uint8_t>({'G','l','o','b','a','l','I','n','f','o',0,1,'S','c','e','n','e','I','n','f','o'}), 'S');
        //sceneInfo.addProperty("GlobalInfo\x00\x01SceneInfo");
        sceneInfo.addProperty("UserData");
        sceneInfo.addPropertyNode("Type", "UserData");
        sceneInfo.addPropertyNode("Version", 100);
        {
            FBXNode metadata("MetaData");
            metadata.addPropertyNode("Version", 100);
            metadata.addPropertyNode("Title", "");
            metadata.addPropertyNode("Subject", "");
            metadata.addPropertyNode("Author", "");
            metadata.addPropertyNode("Keywords", "");
            metadata.addPropertyNode("Revision", "");
            metadata.addPropertyNode("Comment", "");
            sceneInfo.children.push_back(metadata);
        }
        {
            FBXNode properties("Properties70");
            {
                FBXNode p("P");
                p.addProperty("DocumentUrl");
                p.addProperty("KString");
                p.addProperty("Url");
                p.addProperty("");
                p.addProperty("/foobar.fbx");
                properties.children.push_back(p);
            }
            {
                FBXNode p("P");
                p.addProperty("SrcDocumentUrl");
                p.addProperty("KString");
                p.addProperty("Url");
                p.addProperty("");
                p.addProperty("/foobar.fbx");
                properties.children.push_back(p);
            }
            {
                FBXNode p("P");
                p.addProperty("Original");
                p.addProperty("Compound");
                p.addProperty("");
                p.addProperty("");
                properties.children.push_back(p);
            }
            {
                FBXNode p("P");
                p.addProperty("Original|ApplicationVendor");
                p.addProperty("KString");
                p.addProperty("");
                p.addProperty("");
                p.addProperty("Blender Foundation");
                properties.children.push_back(p);
            }
            {
                FBXNode p("P");
                p.addProperty("Original|ApplicationName");
                p.addProperty("KString");
                p.addProperty("");
                p.addProperty("");
                p.addProperty("Blender (stable FBX IO)");
                properties.children.push_back(p);
            }
            {
                FBXNode p("P");
                p.addProperty("Original|ApplicationVersion");
                p.addProperty("KString");
                p.addProperty("");
                p.addProperty("");
                p.addProperty("2.78 (sub 0)");
                properties.children.push_back(p);
            }
            {
                FBXNode p("P");
                p.addProperty("Original|DateTime_GMT");
                p.addProperty("DateTime");
                p.addProperty("");
                p.addProperty("");
                p.addProperty("01/01/1970 00:00:00.000");
                properties.children.push_back(p);
            }
            {
                FBXNode p("P");
                p.addProperty("Original|FileName");
                p.addProperty("KString");
                p.addProperty("");
                p.addProperty("");
                p.addProperty("/foobar.fbx");
                properties.children.push_back(p);
            }
            {
                FBXNode p("P");
                p.addProperty("LastSaved");
                p.addProperty("Compound");
                p.addProperty("");
                p.addProperty("");
                properties.children.push_back(p);
            }
            {
                FBXNode p("P");
                p.addProperty("LastSaved|ApplicationVendor");
                p.addProperty("KString");
                p.addProperty("");
                p.addProperty("");
                p.addProperty("Blender Foundation");
                properties.children.push_back(p);
            }
            {
                FBXNode p("P");
                p.addProperty("LastSaved|ApplicationName");
                p.addProperty("KString");
                p.addProperty("");
                p.addProperty("");
                p.addProperty("Blender (stable FBX IO)");
                properties.children.push_back(p);
            }
            {
                FBXNode p("P");
                p.addProperty("LastSaved|DateTime_GMT");
                p.addProperty("DateTime");
                p.addProperty("");
                p.addProperty("");
                p.addProperty("01/01/1970 00:00:00.000");
                properties.children.push_back(p);
            }
            sceneInfo.children.push_back(properties);
        }
        headerExtension.children.push_back(sceneInfo);
    }
    nodes.push_back(headerExtension);


}

std::uint32_t FBXDocument::getVersion()
{
    return version;
}

void FBXDocument::print()
{
    cout << "{\n";
    cout << "  \"version\": " << getVersion() << ",\n";
    cout << "  \"children\": [\n";
    bool hasPrev = false;
    for(auto node : nodes) {
        if(hasPrev) cout << ",\n";
        node.print("    ");
        hasPrev = true;
    }
    cout << "\n  ]\n}" << endl;
}

} // namespace fbx
