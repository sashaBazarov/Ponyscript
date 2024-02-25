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


#ifndef MYSTRINGFUNCTIONS_H  // Это директива препроцессора, чтобы избежать повторного включения файла
#define MYSTRINGFUNCTIONS_H



int sum(int a, int b);

std::string replaceAll(const std::string& str, const std::string& oldSubstr, const std::string& newSubstr);

#endif

#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H


// Класс для работы с файлами и их содержимым.
// Реализует методы для чтения и записи строк из файла.
// Реализует методы для добавления строк в файл.
// Реализует методы для закрытия файла.
// Реализует метод для проверки открытия файла.
// Реализует метод для проверки закрытия файла.
// Реализует метод
class FileManager {
public:
    FileManager(const std::string& file_name);

    std::string read_line();
    std::vector<std::string> read_lines();

    void write_line(const std::string& line);
    void write_lines(const std::vector<std::string>& lines);

    void append_line(const std::string& line);

    bool is_open() const;
    void close();

private:
    std::string file_name;
    std::fstream file;
};

#endif



void neighln(std::string input);
void neigh(std::string input);



void test_pony_exception();

/**
 * Read a string from standard input with optional prompt.
 * 
 * @param text Optional prompt text to display.
 * @return The string read from standard input.
 */
std::string read(const std::string& text = "");

/**
 * Read a keypress from standard input without echoing to the console, with optional prompt.
 * 
 * @param text Optional prompt text to display.
 */
void readkey();