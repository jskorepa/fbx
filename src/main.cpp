#include <stdint.h>
#include <iostream>
#include <boost/program_options.hpp>

#include "fbxdocument.h"
namespace po = boost::program_options;
using std::cout;
using std::cerr;
using std::endl;
using std::string;

int main(int argc, char** argv) {
    // Declare the supported options.
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "print help message")
        ("output,o", po::value<string>(), "output file name")
        ("input,i", po::value<string>(), "input file name")
    ;

    po::variables_map vm;
    try {
        po::store(po::parse_command_line(argc, argv, desc), vm);
        if (vm.count("help")) {
            cout << desc << "\n";
            return 1;
        }
        po::notify(vm);
    } catch(po::error& e) {
        cerr << "ERROR: " << e.what() << endl << endl;
        cerr << desc << endl;
        return 1;
    }
    if(!vm.count("output") && !vm.count("input")) {
        cerr << "ERROR: Output nor input file was not set." << endl;
        return 1;
    }

    fbx::FBXDocument document;

    if(vm.count("input")) {
        try {
            string input = vm["input"].as<string>();
            cout << input << endl;
            document.read(input);
        } catch(std::string e) {
            cout << e << endl;
            return 1;
        }
    }

    if(vm.count("output")) {
        string output = vm["output"].as<string>();
        cout << output << endl;
    }

    return 0;
}
