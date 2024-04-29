#include <regex>

using namespace std;

bool is_string(string str){
    
    if (str.empty() || str.size() < 2) {
        return false;
    }

    if (str.front() == '"' && str.back() == '"') {
        return true;
    } else {
        return false;
    }


}

bool is_type(string str){
    if(str[0] == '<' && str[-1] == '>'){
        return true;
    }
    else{
        return false;
    }
}

std::string strip(const std::string& str, const char ch) {
    size_t start = str.find_first_not_of(ch);
    size_t end = str.find_last_not_of(ch);
    
    if (start == std::string::npos || end == std::string::npos) {
        return ""; 
    } else {
        return str.substr(start, end - start + 1);
    }
}


bool isDigit(const std::string& str) {
    for (char ch : str) {
        if (!isdigit(ch)) {
            return false;
        }
    }
    return true; 
}

bool is_vector_contain_string(std::vector<std::string> arr, std::string str){
    if(std::find(arr.begin(), arr.end(), str) != arr.end()) return true;
    else return false;
}

std::vector<std::string> split_string(const std::string& input, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(input);

    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}