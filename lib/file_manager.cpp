#include "lib.h"

FileManager::FileManager(const std::string& file_name) : file_name(file_name) {
    file.open(file_name, std::ios::in | std::ios::out | std::ios::app);
}

std::string FileManager::read_line() {
    std::string line;
    std::getline(file, line);
    return line;
}

std::vector<std::string> FileManager::read_lines() {
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    return lines;
}

void FileManager::write_line(const std::string& line) {
    file << line << std::endl;
}

void FileManager::write_lines(const std::vector<std::string>& lines) {
    for (const std::string& line : lines) {
        file << line << std::endl;
    }
}

void FileManager::append_line(const std::string& line) {
    file.clear();
    file.seekp(0, std::ios::end);
    file << line << std::endl;
}

bool FileManager::is_open() const {
    return file.is_open();
}

void FileManager::close() {
    file.close();
}