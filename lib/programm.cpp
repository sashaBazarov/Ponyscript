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

    ofstream tmp;

    tmp.open(".ponytemp");

    if(tmp.is_open())
    {
        tmp << argv[1];
    }
    
    tmp.close();

    std::string path = argv[0];
    std::string toRemove = "a.exe";


    // Получение пути к исполняемому файлу
    std::filesystem::path exePath = std::filesystem::absolute(std::filesystem::path(argv[0]));

    std::filesystem::path exeDir = exePath.parent_path();



    filesystem::path directoryPath = exeDir;
    string stringpath = directoryPath.generic_string();


    string command = stringpath + "/python/python.exe" + " " + stringpath + "/compiler.py";

    system(command.c_str());

    string x = "";
    cin >> x;
}
