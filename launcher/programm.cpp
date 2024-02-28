#include <iostream>
#include <filesystem>
#include <format>


using namespace std;
using std::filesystem::current_path;

int main(int argc, char *argv[]){
    
    bool launch = false;
    
    std::string path = argv[0];

    std::filesystem::path exePath = std::filesystem::absolute(std::filesystem::path(argv[0]));

    std::filesystem::path exeDir = exePath.parent_path();

    filesystem::path directoryPath = exeDir;
    string stringpath = directoryPath.generic_string();

    string command = stringpath + "/python/python.exe" + " " + stringpath + "/compiler.py" + " " + argv[1];

    system(command.c_str());
}
