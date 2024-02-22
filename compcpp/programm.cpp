#include <iostream>
#include <filesystem>
#include <format>
#include <array>
#include <fstream>

using namespace std;
using std::filesystem::current_path;
namespace fs = filesystem;


int main(int argc, char *argv[]){
    
    bool launch = false;
    
    std::string path = argv[0];

    // Получение пути к исполняемому файлу
    std::filesystem::path exePath = std::filesystem::absolute(std::filesystem::path(argv[0]));

    std::filesystem::path exeDir = exePath.parent_path();

    filesystem::path directoryPath = exeDir;
    string stringpath = directoryPath.generic_string();
    
    ofstream tmp;

    tmp.open(stringpath + "/.ponytemp");
    
    if(tmp.is_open())
    {
        tmp << argv[1];
    }
    
    tmp.close();

    string command = stringpath + "/python/python.exe" + " " + stringpath + "/compiler.py" + " " + argv[1];

    system(command.c_str());

}
