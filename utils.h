#include <string>
#include <vector>



bool is_string(std::string str);
bool is_type(std::string str);
std::string strip(const std::string& str, const char ch);
bool isDigit(const std::string& str);
bool is_vector_contain_string(std::vector<std::string> arr, std::string str);
std::vector<std::string> split_string(const std::string& input, char delimiter);