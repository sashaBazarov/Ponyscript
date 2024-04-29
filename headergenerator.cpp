#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <regex>


void extract_functions_constants_classes(const std::string& file_path, 
std::vector<std::pair<std::string, std::string>>& functions,std::vector
<std::pair<std::string, std::string>>& constants,
std::vector<std::string>& classes,
std::set<std::string>& includes){
std::ifstream file(file_path);
if (!file.is_open()) {
    std::cerr << "Failed to open file: " << file_path << std::endl;
    return;
}

std::string line;
bool in_comment_block = false;
std::string current_function = "";
std::string current_class = "";

std::regex function_regex("^\\s*([\\w:<>]+)\\s+([\\w:<>]+)\\s*\\((.*)\\)\\s*");
std::regex function_regex2("^\\s*([\\w:<>]+)\\s+([\\w:<>]+)\\s*\\((.*)\\)\\s*\\{");
std::regex constant_regex("^\\s*const\\s+([\\w:<>]+)\\s+([\\w]+)\\s*=\\s*([^;]+);");
std::regex constant_regex2("^\\s*constexpr\\s+([\\w:<>]+)\\s+([\\w]+)\\s*=\\s*([^;]+);");
std::regex class_regex("^\\s*class\\s+([\\w:<>]+)");
std::regex include_regex("^\\s*#include\\s+<(.*)>");
std::regex include_regex2("^\\s*#include\\s+\"(.*)\"");
    
    while (getline(file, line)) {
        // Check for comment blocks
        if (line.find("/*") != std::string::npos) {
            in_comment_block = true;
        }
        if (line.find("*/") != std::string::npos) {
            in_comment_block = false;
            continue;
        }
        
        if (!in_comment_block) {
            // Extract functions
            std::smatch match;
            if (regex_match(line, match, function_regex) || regex_match(line, match, function_regex2)) {
                std::string return_type = match[1];
                std::string function_name = match[2];
                std::string arguments = match[3];
                functions.emplace_back(return_type, function_name + "("+ arguments +")");
            }
            
            // Extract constants 
            if (regex_match(line, match, constant_regex) || regex_match(line, match, constant_regex2)) {
                std::string constant_type = match[1];
                std::string constant_name = match[2];
                std::string constant_value = match[3];
                constants.emplace_back(constant_type, constant_name);
            }
            
            // Extract classes
            if (regex_match(line, match, class_regex)) {
                std::string class_name = match[1];
                classes.push_back(class_name);
            }
            
            // Extract includes
            if (regex_match(line, match, include_regex) || regex_match(line, match, include_regex2)) {
                includes.insert(match[1]);
            }
        }
    }
}

void write_to_header(const std::string& header_path,
                     const std::vector<std::pair<std::string, std::string>>& functions,
                     const std::vector<std::pair<std::string, std::string>>& constants,
                     const std::vector<std::string>& classes,
                     const std::set<std::string>& includes) {
    std::ofstream header_file(header_path);

    if (!header_file.is_open()) {
        std::cerr << "Failed to open file for writing: " << header_path << std::endl;
        return;
    }

    for (const auto& include : includes) {
        header_file << "#include \"" << include << "\"\n";
    }

    header_file << "\n";

    for (const auto& constant : constants) {
        header_file << "constexpr " << constant.first << " " << constant.second << ";\n";
    }

    if (!constants.empty()) {
        header_file << "\n";
    }

    for (const auto& function : functions) {
        header_file << function.first << " " << function.second << ";\n";
    }

    if (!functions.empty()) {
        header_file << "\n";
    }

    for (const auto& class_name : classes) {
        header_file << "class " << class_name << ";\n";
    }
}

void generate_cpp_header(std::string cpp_file_path, std::string header_file_path) {
    std::vector<std::pair<std::string, std::string>> functions;
    std::vector<std::pair<std::string, std::string>> constants;
    std::vector<std::string> classes;
    std::set<std::string> includes;

    extract_functions_constants_classes(cpp_file_path, functions, constants, classes, includes);
    write_to_header(header_file_path, functions, constants, classes, includes);
}



