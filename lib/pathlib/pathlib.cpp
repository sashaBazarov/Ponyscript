#include <string>
#include "ponystring.h"
#include <filesystem>

namespace pathlib{


bool make_directory(ponystring path){
    std::string directoryName = path;

    try {
        std::filesystem::create_directory(directoryName);
        return true;
    } catch (const std::filesystem::filesystem_error& e) {
        return false;
    }
    }

bool remove_directory(ponystring path){
    std::string directoryName = path;

    try {
        std::filesystem::remove_all(directoryName);
        return true;
    } catch (const std::filesystem::filesystem_error& e) {
        return false;
    }

}

bool deleteAllFilesInDirectory(const ponystring& directory) {
    bool success = true;
    std::string directoryName = directory;
    std::filesystem::path dirPath(directoryName.c_str()); // Преобразование ponystring в std::string, а затем в std::filesystem::path
    for (const auto& entry : std::filesystem::directory_iterator(dirPath)) {
        if (std::filesystem::is_regular_file(entry.path())) {
            if (!std::filesystem::remove(entry.path())) {
                
                success = false;
            } else {
                
            }
        }
    }
    return success;
}

bool copyFile(const ponystring& source, const ponystring& destination) {
    try {
        std::string sourceStr = source;
        std::string destinationStr = destination;
        std::filesystem::copy_file(sourceStr, destinationStr, std::filesystem::copy_options::overwrite_existing); 
        return true;
    } catch (const std::filesystem::filesystem_error& e) {
        return false;
    }
}

bool moveFile(const ponystring& source, const ponystring& destination) {
    try {
        std::string sourceStr = source;
        std::string destinationStr = destination;
        std::filesystem::rename(sourceStr, destinationStr);
        return true;
    } catch (const std::filesystem::filesystem_error& e) {
        return false;
    }
}

bool isFile(const ponystring& filePath) {
    return std::filesystem::is_regular_file(filePath.str());
}
}