#include <regex>
#include <vector>


std::vector<std::string> find_includes(std::vector<std::string> &tokens);
std::vector<std::string> find_classes(std::vector<std::string> &tokens);
std::vector<std::string> find_namespaces(std::vector<std::string> &tokens);
std::vector<std::string> find_functions(std::vector<std::string> &tokens);
std::vector<std::string> find_variables(std::vector<std::string> &tokens, std::vector<std::string> classes);
std::vector<std::string> format_strings(std::vector<std::string> &tokens);
std::vector<std::string> remove_comments(const std::vector<std::string>& tokens);
std::vector<std::string> translate_tokens(std::vector<std::string> &tokens, std::vector<std::string> variables, std::vector<std::string> classes, std::vector<std::string> namespaces, std::vector<std::string> includes);
std::vector<std::string> lexical_analyzer(std::string str);
std::string assemble_tokens(std::vector<std::string> tokens);


