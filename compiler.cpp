#include <string>
#include <iostream>
#include <filesystem>
#include <vector>
#include <fstream>
#include <unistd.h>
#include <cstdlib>
#include <array>

#include "lexer.h"
#include "headergenerator.h"
#include "libparcer.h"
#include "utils.h"
#include "gccdockparcer.h"

namespace fs = std::filesystem;
using namespace std;

void remove_files_and_exit(int exit_code, string bin_directory){

    for (const auto & entry : fs::directory_iterator(bin_directory))
        {
            std::string file_path = entry.path().u8string();
        if (file_path.substr(file_path.find_last_of(".") + 1) == "o"){
            fs::remove(file_path);
        }
    }
    exit(exit_code);
}

std::pair<int, std::string> execute_command(const std::string& command) {
    FILE* pipe = _popen((command + " 2>&1").c_str(), "r");
    if (!pipe) {
        throw std::runtime_error("Ошибка при открытии pipe.");
    }
    char buffer[128];
    std::string result;
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }
    int exit_code = _pclose(pipe);
    return make_pair(exit_code, result);
}


int main(int argc, char *argv[])
{
    std::filesystem::path exePath = std::filesystem::absolute(std::filesystem::path(argv[0]));

    std::filesystem::path exeDir = exePath.parent_path();

    filesystem::path directoryPath = exeDir;
    string stringpath = directoryPath.generic_string(); 

    std::vector<std::string> filestocompile;
    std::string dir_path = argv[1];
    std::vector<std::string> buildfiles;
    std::vector<std::string> included_libs;

    vector<string> secondary_build_files;

    string exit_file_name;

    vector<string> libs = get_libs(stringpath + "/lib");

    build_libs(stringpath + "/lib");

    ifstream cfg_file(stringpath + "/.ponycfg");
    string cfg_line ="";

    for (int i = 1; i < argc; i++) {
        std::cout << argv[i] << std::endl;
        if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            exit_file_name = argv[i + 1];
            i++;
        }
    }

    for (const auto & entry : fs::directory_iterator(dir_path))
        {
            std::string file_path = entry.path().u8string();
        if (file_path.substr(file_path.find_last_of(".") + 1) == "psc"){
            filestocompile.push_back(file_path);
        }
    }

    fs::remove_all(dir_path + "/bin");
    if(!fs::is_directory(dir_path + "/bin"))
    {
        fs::create_directory(dir_path + "/bin");
        fs::create_directory(dir_path + "/bin/lib");
    }
        
    for (std::string file : filestocompile){

        std::ifstream file_stream(file);

        std::string file_content;
        std::string line;
        while(std::getline(file_stream, line)){
            file_content += line + "\n";
        }
        line.clear();
        file_stream.close();

        std::vector<std::string> tokens = lexical_analyzer(file_content);
        file_content.clear();

        file = file.substr(file.find_last_of("/"));

        std::string file_name = fs::path(file).stem().u8string();

        std::vector<std::string> includes =  find_includes(tokens);

        for(string include : includes){
            if(is_vector_contain_string(libs, include) && !is_vector_contain_string(included_libs, include))
                included_libs.push_back(include);
        }

        vector<string> classes = find_classes(tokens);

        vector<string> functions = find_functions(tokens);

        vector<string> variables = find_variables(tokens, classes);

        format_strings(tokens);

        vector<string> namespaces = find_namespaces(tokens);

        translate_tokens(tokens, variables, classes, namespaces, includes);

        std::string output_file_content = "";

        output_file_content += "#include \"lib.h\" \n";

        output_file_content += "#include \"linked_list.h\" \n";

        output_file_content += "#define FILE_INFO " + file_name + ".psc \n";

        std::string output_file_path =  dir_path + "/bin/" + file_name + ".cpp";

        ofstream output_file(output_file_path);

        if(output_file.is_open())
        {

            output_file_content += assemble_tokens(tokens);

            output_file << output_file_content;

            output_file.close();

            tokens.clear();

            std::string header_file_path = dir_path + "/bin/" + file_name + ".h";

            generate_cpp_header(output_file_path, header_file_path);

            fprintf(stderr, "Compiled %s\n", file.c_str());
        }else{
            cout << "Error opening file!" << endl;
        }

        buildfiles.push_back(dir_path + "/bin/" + file_name + ".cpp");

        output_file.clear();
        output_file_content.clear();
        file_stream.clear();

    }

    fs::copy(stringpath + "/lib/run.cpp", dir_path + "/bin/lib/run.cpp");
    fs::copy(stringpath + "/lib/run.h", dir_path + "/bin/lib/run.h");

    secondary_build_files.push_back(dir_path + "/bin/lib/run.cpp");

    string libs_path_arguments = "";

    for(string lib : included_libs){
        libs_path_arguments += "-I" + stringpath + "/lib/" + lib + " ";
        secondary_build_files.push_back(stringpath + "/lib/" + lib + "/" + lib +".cpp");
    }

    string build_arguments = "-L" + stringpath +"/lib -I" + stringpath+ "/lib " + libs_path_arguments + " -lponylib -finput-charset=UTF-8 -lstdc++";

    for(string file : buildfiles){
        std::string file_name = fs::path(file).stem().u8string();

        cout << "Compiling file " + file << endl;
        pair<int, string> builder_output = execute_command(stringpath + "/ucrt64/bin/g++.exe -c " + file + " -o " + dir_path + "/bin/" + file_name + ".o " + build_arguments);

        string output = builder_output.second;
        int exit_code = builder_output.first;

        cout << parse(output) << endl;

        if(exit_code != 0){
            cout << "Error compiling file!" << endl;
            remove_files_and_exit(1, dir_path + "/bin");
        }

        secondary_build_files.push_back(dir_path + "/bin/" + file_name + ".o");
    }
    
    string secondary_build_files_string;
    for(string file : secondary_build_files){
        secondary_build_files_string += file + " ";
    }

    cout << "Executable file name " + exit_file_name << endl;
    cout << "Building executable..." << endl;

    string secondray_build_command = stringpath + "/ucrt64/bin/g++ " + secondary_build_files_string + " -o " + dir_path + "/bin/"  + exit_file_name + " " + build_arguments;

    pair<int, string> output_pair = execute_command(secondray_build_command);

    string output = output_pair.second;
    int exit_code = output_pair.first;

    cout << parse(output)<< endl;


    if(exit_code != 0){
        cout << "Error building executable!" << endl;
        remove_files_and_exit(1, dir_path + "/bin");
    }
    

    cout << "Executable built successfully!" << endl;

    remove_files_and_exit(0, dir_path + "/bin");

    return 0;

}
