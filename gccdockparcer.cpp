#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

// Константа для количества дополнительных строк
const int default_additional_lines = 4;

// Функция для извлечения путей файлов
std::vector<std::string> extract_file_paths(const std::string& text) {
    // Регулярное выражение для поиска путей файлов
    std::regex filepath_regex("[a-zA-Z]:?[^:\\n]+:\\d+:\\d+");
    std::smatch match;
    std::vector<std::string> file_paths;

    // Поиск всех совпадений в тексте
    auto words_begin = std::sregex_iterator(text.begin(), text.end(), filepath_regex);
    auto words_end = std::sregex_iterator();

    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        match = *i;
        file_paths.push_back(match.str());
    }

    return file_paths;
}

// Функция для разбора строки файла
std::tuple<std::string, int, int> parse_file_line(const std::string& line) {
    // Регулярное выражение для разбора строки файла
    std::regex pattern("^(.*?):(\\d+):(\\d+)$");
    std::smatch match;

    // Попытка найти соответствие регулярному выражению
    if (std::regex_match(line, match, pattern)) {
        std::string file_path = match[1];
        int line_number = std::stoi(match[2]);
        int char_number = std::stoi(match[3]);
        return std::make_tuple(file_path, line_number, char_number);
    } else {
        // Возврат пустых значений в случае неудачи
        return std::make_tuple("", 0, 0);
    }
}

std::string parse(const std::string& logs) {
    std::string out;
    std::vector<std::string> lines;

    // Разбиваем логи на строки
    std::istringstream iss(logs);
    std::string line;
    while (std::getline(iss, line)) {
        lines.push_back(line);
    }

    std::vector<std::string> file_paths;
    for (const auto& line : lines) {
        if (!line.empty()) {
            auto paths = extract_file_paths(line);
            file_paths.insert(file_paths.end(), paths.begin(), paths.end());
        }
    }

    // Итерация по найденным путям файлов
    for (const auto& file_path : file_paths) {
        std::string path;
        int ln, let;
        std::tie(path, ln, let) = parse_file_line(file_path);
        if (!path.empty() && path.substr(path.find_last_of(".") + 1) != "run.cpp") {
            if (path.substr(path.find_last_of(".") + 1) == "cpp") {
                path.replace(path.find_last_of(".") + 1, 3, "psc");
            }
            if (path.substr(path.find_last_of(".") + 1) == "h") {
                path.replace(path.find_last_of(".") + 1, 1, "psh");
            }
            
            for (const auto& line : lines) {
                std::vector<std::string> tokens;
                std::istringstream iss(line);
                std::string token;
                while (std::getline(iss, token, ':')) {
                    tokens.push_back(token);
                }

                if (std::find(tokens.begin(), tokens.end(), "error") != tokens.end()) {
                    out += "\033[1;31m" + line + "\033[0m\n"; // Красный цвет для ошибок
                }

                if (line.find(file_path) != std::string::npos) {
                    size_t pos = line.find(file_path);
                    std::string temp = line;
                    out += "\n" + temp.replace(pos, file_path.size(), "File: " + path.substr(path.find_last_of("/") + 1) + " | Line: " + std::to_string(ln - default_additional_lines) + " \n").substr(0, temp.find('\n')) + "\n" + temp.replace(pos, file_path.size(), "File: " + path.substr(path.find_last_of("/") + 1) + " | Line: " + std::to_string(ln - 3) + " \n").substr(1).erase(0, 1);
                }
            }
        }
    }

    return out;
}
