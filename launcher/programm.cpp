#include <iostream>
#include <filesystem>
#include <format>
#include <fstream>


using namespace std;
using std::filesystem::current_path;

int main(int argc, char *argv[]){
    
    std::filesystem::path exePath = std::filesystem::absolute(std::filesystem::path(argv[0]));

    std::filesystem::path exeDir = exePath.parent_path();

    filesystem::path directoryPath = exeDir;
    string stringpath = directoryPath.generic_string(); 

    if (argv[0] == "version")
    {

        cout << "Version: 0.0.1 alpha" << endl;
        return 0;
    }


    if (argv[0] == "libinfo")
    {
        std::ifstream file;
        file.open(stringpath + "/lib/" + argv[1] + "/libinfo"); 
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
