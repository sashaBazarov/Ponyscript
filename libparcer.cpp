#include <filesystem>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <fstream>

#include "lexer.h"
#include "utils.h"
#include "headergenerator.h"

namespace fs = std::filesystem;
using namespace std;

std::vector<std::string> get_libs(std::string path_to_libraries){
    fs::path p(path_to_libraries);
    std::vector<std::string> libs;
    for(auto& x : fs::directory_iterator(p)){
        if(fs::is_directory(x.path())){
            std::string path = x.path().string(); 
            std::replace(path.begin(), path.end(), '\\', '/');
            if(fs::exists(path+"/libinfo"))
                libs.push_back(path.substr(path.find_last_of("/")+1));
        }
    }
    return libs;
}


void build_libs(std::string path_to_libraries){
    std::vector<std::string> libs = get_libs(path_to_libraries);

    for(auto& lib : libs){
        std::string library_path = path_to_libraries+"/"+lib;

        if(!fs::exists(library_path+"/" + lib + ".cpp") && !fs::exists(library_path+"/" + lib + ".h") && fs::exists(library_path+"/" + lib + ".psc") && fs::exists(library_path+"/" + lib + ".psh")){
             
            cout << "Building " << lib << endl;

            std::ifstream file_stream(library_path+"/"+lib+".psc");

            std::string file_content;
            std::string line;
            while(std::getline(file_stream, line)){
                file_content += line + "\n";
            }

            line.clear();
            file_stream.close();

            vector<string> tokens = lexical_analyzer(file_content);

            string file_name = lib;

            vector<string> included_libs;


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

            std::string output_file_path = library_path + "/" + file_name + ".cpp";

            ofstream output_file(output_file_path);

            if(output_file.is_open())
            {
                output_file_content += assemble_tokens(tokens);

                output_file << output_file_content;

                output_file.close();

                tokens.clear();

                std::string header_file_path = library_path + "/" + file_name + ".h";

                generate_cpp_header(output_file_path, header_file_path);


            }

        }
    }
}
