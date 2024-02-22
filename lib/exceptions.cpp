#include <iostream>
#include <exception>
#include <string>
#include "lib.h"



#define FILE_INFO std::string(replaceAll(__FILE__, ".cpp", ".psc"))
#define LINE_INFO std::to_string(__LINE__)

class PonyException : public std::exception {
public:
    PonyException(const std::string& message, const std::string& file, int line) 
        : errorMessage(message + "\n" +"In file: " + file + " Line: " + std::to_string(line)), file(file), line(line) {}

    virtual const char* what() const noexcept override {
        return errorMessage.c_str();
    }

    const char* getFile() const noexcept {
        return file.c_str();
    }

    int getLine() const noexcept {
        return line;
    }

private:
    std::string errorMessage;
    std::string file;
    int line;
};

