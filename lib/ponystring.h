#ifndef PONYSTRING_H
#define PONYSTRING_H

#include <iostream>
#include <vector>
#include <sstream>

class ponystring {
private:
    std::string data;

public:
    ponystring() {}
    ponystring(const char* str) : data(str) {}
    ponystring(const std::string& str) : data(str) {}
    ponystring(const ponystring& other) : data(other.data) {}

    ponystring& operator=(const char* str);
    ponystring& operator=(const std::string& str);
    ponystring& operator=(const ponystring& other);

    bool operator!=(const ponystring& other) const;
    bool operator!=(const std::string& other) const;
    bool operator!=(char ch) const;

    ponystring& operator+=(const char* str);
    ponystring& operator+=(const std::string& str);
    ponystring& operator+=(const ponystring& other);

    ponystring operator+(const ponystring& other) const;

    // Функция возвращает константный итератор на начало строки
    std::string::const_iterator cbegin() const;

    // Функция возвращает константный итератор на конец строки
    std::string::const_iterator cend() const;

    char& operator[](size_t index);
    const char& operator[](size_t index) const;

    operator std::string() const { return data; }

    friend std::ostream& operator<<(std::ostream& os, const ponystring& str);
    friend std::istream& operator>>(std::istream& is, ponystring& str);

    std::vector<ponystring> split(char delimiter = ' ') const;

    ponystring& removeExtraSpaces();

    ponystring& replaceSubstring(const ponystring& oldSubstr, const ponystring& newSubstr);

    ponystring& strip(const std::string& chars = " \t\r\n");

    static ponystring fromInt(int value);
    static ponystring fromDouble(double value);

        // Функция работы с длиной строки
    size_t length() const;

    // Функции объединения строк
    ponystring concat(const ponystring& other) const;

    // Функции сравнения строк
    bool equals(const ponystring& other) const;

    // Функции поиска подстроки
    size_t indexOf(const ponystring& substr) const;

    // Функции обрезки строк
    ponystring substring(size_t start, size_t length) const;

    // Функции замены подстроки
    ponystring replace(const ponystring& oldSubstr, const ponystring& newSubstr) const;

    // Функции преобразования регистра
    ponystring toUpperCase() const;
    ponystring toLowerCase() const;

    // Функции удаления пробелов
    ponystring trim() const;

    // Вспомогательные функции
    const std::string& str() const { return data; }

    //Проверка на путую строку
    bool isEmpty() const;

    bool isDigit() const;
};

#endif // PONYSTRING_H