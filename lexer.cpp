#include <regex>
#include <vector>
#include "utils.h"
#include "lexer.h"
#include "wordlist.h"

const std::vector<std::string> variable_types = {"int", "float", "double", "char", "bool", "string"};

std::vector<std::string> find_includes(std::vector<std::string> &tokens){
    std::vector<std::string> includes;
    for(int i = 0; i < tokens.size(); i++){
        if(tokens[i] == "friend"){
            includes.push_back(tokens[i+1]);
            tokens[i+1] = "\"" + tokens[i+1] + ".h" + "\"";
        }
    }
    return includes;
} 

std::vector<std::string> find_classes(std::vector<std::string> &tokens){
    std::vector<std::string> classes;
    std::vector<std::pair<int, std::string>> indexes_to_insert;
    

    for(int i = 0; i < tokens.size(); i++){
        if(tokens[i] == "class"){
          classes.push_back(tokens[i+1]);
          int arguments_start = i+2;
          int arguments_end = i+2;
          int j =0;
          while(i+j < tokens.size()){
            j++;
            if(tokens[i+j] == "{"){
                indexes_to_insert.push_back(std::make_pair(i + j, "public: \n"));
                arguments_end = i+j-1;
            }
            std::string arguments = "";
            for(int k = arguments_start; k <= arguments_end; k++){
                arguments += tokens[k] + " ";
            }
            indexes_to_insert.push_back(std::make_pair(i + j+1, arguments));
            arguments.clear();

          }
        }
    }


    for(int i = 0; i < indexes_to_insert.size(); i++){
        tokens.insert(tokens.begin() + indexes_to_insert[i].first, indexes_to_insert[i].second);
    }

    indexes_to_insert.clear();

    return classes;
}

std::vector<std::string> find_namespaces(std::vector<std::string> &tokens){
    std::vector<std::string> namespaces;
    for(int i = 0; i < tokens.size(); i++){
        if(tokens[i] == "namespace"){
          namespaces.push_back(tokens[i+1]);
        }
    }
    return namespaces;
}

std::vector<std::string> find_functions(std::vector<std::string> &tokens){
    std::vector<std::string> functions;
    for(int i = 0; i < tokens.size(); i++){
        if(tokens[i] == "("){
          functions.push_back(tokens[i-1]);
        }
    }
    return functions;
}

std::vector<std::string> find_variables(std::vector<std::string> &tokens, std::vector<std::string> classes){
    std::vector<std::string> variables;
    for(int i = 0; i < tokens.size(); i++){
        if(std::find(variable_types.begin(), variable_types.end(), tokens[i]) != variable_types.end()){
            int j = 1;
            while(tokens[i+j] == "*"){
                j++;
            }
            variables.push_back(tokens[i+j]);
        } 
        else if(std::find(classes.begin(), classes.end(), tokens[i]) != classes.end()){
            int j = 1;

            if(tokens[i+1] == "<"){
                while(tokens[i+j] != ">"){
                    j++;
                }
            }

            while(tokens[i+j] == "*"){
                j++;
            }
            variables.push_back(tokens[i+j]);
        }

    }
    return variables;
}

std::vector<std::string> format_strings(std::vector<std::string> &tokens){
    std::vector<std::string> updated_tokens = tokens;
    std::vector<int> indexes_to_remove;
    std::vector<int> indexes_to_insert;

    for(int i = 0; i < tokens.size(); i++){

        if(is_string(tokens[i]) && tokens[i-1] == "f"){
            std::string str = strip(tokens[i], '\"');
            std::string formatted_str = "\"\"";

            int last_index = 0;
            for(int j = 0; j < str.size(); j++){
                if(str[j] == '{'){
                    formatted_str  += " + \"" + str.substr(last_index, j - last_index) +"\"";
                    int k = j;
                    while(k < str.size() && str[k] != '}'){
                        k++;
                    }
                    std::string code_string = str.substr(j + 1, k - j - 1);
                    std::vector<std::string> code_tokens = lexical_analyzer(code_string);
                    std::string code_str = assemble_tokens(format_strings(code_tokens));
                    formatted_str += " + ponystring(" + code_str + ")";
                    last_index = k + 1;
                }
            }

            formatted_str = " ponystring(" + formatted_str + ")";



            updated_tokens[i] = formatted_str;
            indexes_to_remove.push_back(i-1);
        }
    }

    for(int i = 0; i < indexes_to_remove.size(); i++){
        updated_tokens.erase(updated_tokens.begin() + indexes_to_remove[i]);
    }

    tokens = updated_tokens;
    return tokens;
}

std::vector<std::string> translate_tokens(std::vector<std::string> &tokens, std::vector<std::string> variables, std::vector<std::string> classes, std::vector<std::string> namespaces, std::vector<std::string> includes){
    std::vector<std::string> symbols = {"+", "-", "*", "/", "=", ".", ",", ";", "(", ")", "[", "]", "{", "}", ":", "<", ">"};
    for(int i = 0; i < tokens.size(); i++){
        if (!is_vector_contain_string(variables, tokens[i]) && tokens[i+1] != "(" && !is_vector_contain_string(classes, tokens[i]) 
        && !is_vector_contain_string(namespaces, tokens[i]) && !is_vector_contain_string(includes, tokens[i]) 
        && !is_vector_contain_string(symbols, tokens[i]) && tokens[i] != "\n" && !isDigit(tokens[i]) && !is_string(tokens[i]) && !is_type(tokens[i])) 
        {
            if(wordlist.find(tokens[i]) != wordlist.end()){
                tokens[i] = wordlist.at(tokens[i]);
            }
        }
    }
    return tokens;
}

std::vector<std::string> lexical_analyzer(std::string str)
{
std::regex reg("<.*?>|\\b(?:int|#include|using|namespace|void|return|string|public|private|protected|byte|bool|neigh|read|class)\\b|\\w+|\".*?\"|[.,!?;:(){}\\[\\]=+\\-*/<>]|[\n]");

std::sregex_iterator begin(str.begin(), str.end(), reg);
std::sregex_iterator end;
std::vector<std::string> tokens;
for (std::sregex_iterator it = begin; it != end; ++it) {
    std::smatch match = *it;
    tokens.push_back(match.str());
}
return tokens;
}


std::string assemble_tokens(std::vector<std::string> tokens){
    std::string assembled_code = "";
    std::string last_token = "";
    std::vector<std::string> symbols = {"+", "-", "*", "/", "=", ".", ",", ";", "(", ")", "[", "]", "{", "}", ":", "<", ">"};

    for(std::string token : tokens){

        if(token == "\n")
            assembled_code += "\n";
        else if (is_string(token) && last_token != "#include")
            assembled_code += "ponystring(" + token + ")";
        else if (assembled_code[-1] == '\n')
            assembled_code += token;
        else if(std::find(symbols.begin(), symbols.end(), token) != symbols.end())
            assembled_code += token;
        else if(isDigit(token) && last_token == ".")
            assembled_code += " " + token;
        else 
            assembled_code += " " + token;

        last_token = token;

    }
    return assembled_code;
}

std::vector<std::string> remove_comments(const std::vector<std::string>& tokens) {
    std::string code = assemble_tokens(tokens);
    code = std::regex_replace(code, std::regex("//.*"), "");
    code = std::regex_replace(code, std::regex("/\\*(.*?)\\*/"), "", std::regex_constants::format_first_only);
    return lexical_analyzer(code);
}


