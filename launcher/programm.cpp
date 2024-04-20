#include <iostream>
#include <filesystem>
#include <format>
#include <fstream>
#include <string>
#include <cctype>

using namespace std;
using std::filesystem::current_path;

std::string strip(const std::string &inpt)
{
    auto start_it = inpt.begin();
    auto end_it = inpt.rbegin();
    while (std::isspace(*start_it))
        ++start_it;
    while (std::isspace(*end_it))
        ++end_it;
    return std::string(start_it, end_it.base());
}


int main(int argc, char *argv[]){
    
    std::filesystem::path exePath = std::filesystem::absolute(std::filesystem::path(argv[0]));

    std::filesystem::path exeDir = exePath.parent_path();

    filesystem::path directoryPath = exeDir;
    string stringpath = directoryPath.generic_string(); 

    if (strip(argv[1]) == "version")
    {

        cout << "Version: 0.0.2 alpha" << endl;
        return 0;
    }

    if (strip(argv[1]) == "libinfo")
    {
        std::ifstream file;
        file.open(stringpath + "/lib/" + strip(argv[2]) + "/libinfo"); 
        if (file.is_open())
        {
            string line;
            while (getline(file, line))
            {
                cout << line << endl;
            }
        }

        return 0;

    }

    std::string path = argv[0];

    string command = stringpath + "/python/python.exe" + " " + stringpath + "/compiler.py" + " " + argv[1];

    system(command.c_str());
    
}
