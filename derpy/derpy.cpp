#include <iostream>
#include <filesystem>
#include <fstream>


using namespace std;
using std::filesystem::current_path;

int main(int argc, char *argv[]){
    
    std::filesystem::path exePath = std::filesystem::absolute(std::filesystem::path(argv[0]));

    std::filesystem::path exeDir = exePath.parent_path();

    filesystem::path directoryPath = exeDir;
    string stringpath = directoryPath.generic_string(); 

    std::string path = argv[0];

    std::string args = "";

    for(int i = 1; i < argc; i++){
        args += argv[i];
        args += " ";
    }

    string command = stringpath + "/python/python.exe" + " " + stringpath + "/derpy/derpy.py" + " " + args;

    system(command.c_str());
    
}
