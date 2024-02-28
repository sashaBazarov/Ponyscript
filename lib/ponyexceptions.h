#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <locale>
#include <functional>
#include <string>
#include <exception>


//Класс исключения для обработки ошибок в программе.
//Реализует методы для получения сообщения об ошибке, локации и строки, в которой обнаружена ошибка.
//Реализует методы для получения имени файла, в котором обнаружена ошибка.
//Реализует методы для получения номера строки, в которой обнаружена ошибка.
#ifndef PONY_EXCEPTION_H
#define PONY_EXCEPTION_H


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

class PonyException_Index_out_of_range : public std::exception {
public:
    PonyException_Index_out_of_range(const std::string& message, const std::string& file, int line) 
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


#endif // PONY_EXCEPTION_H
