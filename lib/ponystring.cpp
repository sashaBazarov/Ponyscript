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

    // Операторы присваивания и конкатенации строк
    ponystring& operator=(const char* str);
    ponystring& operator=(const std::string& str);
    ponystring& operator+=(const char* str);
    ponystring& operator+=(const std::string& str);
    ponystring operator+(const ponystring& other) const;

    std::string::const_iterator cbegin() const {
        return data.cbegin();
    }

    // Функция возвращает константный итератор на конец строки
    std::string::const_iterator cend() const {
        return data.cend();
    }

    // Оператор индексации
    char& operator[](size_t index);
    const char& operator[](size_t index) const;

    // Перегрузка операторов вывода и ввода
    friend std::ostream& operator<<(std::ostream& os, const ponystring& str);
    friend std::istream& operator>>(std::istream& is, ponystring& str);

    // Разделение строки на подстроки
    std::vector<ponystring> split(char delimiter = ' ') const;

    // Удаление лишних пробелов
    ponystring& removeExtraSpaces();

    // Замена подстроки
    ponystring& replaceSubstring(const ponystring& oldSubstr, const ponystring& newSubstr);

    // Удаление пробельных символов в начале и конце строки
    ponystring& strip(const std::string& chars = " \t\r\n");

    // Функции, возвращающие ponystring
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

    bool isEmpty() const{
        return data.empty();
    }

    bool operator!=(const ponystring& other) const {
        return data != other.data;
    }

    bool operator!=(char ch) const {
        return data != std::string(1, ch);
    }

    bool operator!=(const std::string& other) const {
        return data != other;
    }

    bool isDigit() const {
        for (char c : data) {
            if (!std::isdigit(c)) {
                return false;
            }
        }
        return true;
    }

};

ponystring& ponystring::operator=(const char* str) {
    data = str;
    return *this;
}

ponystring& ponystring::operator=(const std::string& str) {
    data = str;
    return *this;
}

ponystring& ponystring::operator+=(const char* str) {
    data += str;
    return *this;
}

ponystring& ponystring::operator+=(const std::string& str) {
    data += str;
    return *this;
}

ponystring ponystring::operator+(const ponystring& other) const {
    return ponystring(data + other.data);
}

char& ponystring::operator[](size_t index) {
    return data[index];
}

const char& ponystring::operator[](size_t index) const {
    return data[index];
}

std::ostream& operator<<(std::ostream& os, const ponystring& str) {
    os << str.data;
    return os;
}

std::istream& operator>>(std::istream& is, ponystring& str) {
    is >> str.data;
    return is;
}

std::vector<ponystring> ponystring::split(char delimiter) const {
    std::vector<ponystring> tokens;
    std::istringstream iss(data);
    ponystring token;
    while (std::getline(iss, token.data, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

ponystring& ponystring::removeExtraSpaces() {
    std::string result;
    bool spaceFound = false;
    for (char c : data) {
        if (c == ' ') {
            if (!spaceFound) {
                result.push_back(c);
                spaceFound = true;
            }
        } else {
            result.push_back(c);
            spaceFound = false;
        }
    }
    data = result;
    return *this;
}

ponystring& ponystring::replaceSubstring(const ponystring& oldSubstr, const ponystring& newSubstr) {
    size_t pos = data.find(oldSubstr.data);
    while (pos != std::string::npos) {
        data.replace(pos, oldSubstr.data.length(), newSubstr.data);
        pos = data.find(oldSubstr.data, pos + newSubstr.data.length());
    }
    return *this;
}

ponystring& ponystring::strip(const std::string& chars) {
    size_t start = data.find_first_not_of(chars);
    size_t end = data.find_last_not_of(chars);
    if (start != std::string::npos && end != std::string::npos) {
        data = data.substr(start, end - start + 1);
    } else {
        data.clear();
    }
    return *this;
}

ponystring ponystring::fromInt(int value) {
    return ponystring(std::to_string(value));
}

ponystring ponystring::fromDouble(double value) {
    return ponystring(std::to_string(value));
}

// Функция работы с длиной строки
size_t ponystring::length() const {
    return data.length();
}

// Функции объединения строк
ponystring ponystring::concat(const ponystring& other) const {
    return ponystring(data + other.data);
}

// Функции сравнения строк
bool ponystring::equals(const ponystring& other) const {
    return data == other.data;
}

// Функции поиска подстроки
size_t ponystring::indexOf(const ponystring& substr) const {
    size_t found = data.find(substr.data);
    return (found != std::string::npos) ? found : -1;
}

// Функции обрезки строк
ponystring ponystring::substring(size_t start, size_t length) const {
    return ponystring(data.substr(start, length));
}

// Функции замены подстроки
ponystring ponystring::replace(const ponystring& oldSubstr, const ponystring& newSubstr) const {
    std::string result = data;
    size_t index = 0;
    while ((index = result.find(oldSubstr.data, index)) != std::string::npos) {
        result.replace(index, oldSubstr.length(), newSubstr.data);
        index += newSubstr.length();
    }
    return ponystring(result);
}

// Функции преобразования регистра
ponystring ponystring::toUpperCase() const {
    std::string result = data;
    for (char& c : result) {
        if (std::islower(c)) {
            c = std::toupper(c);
        }
    }
    return ponystring(result);
}

ponystring ponystring::toLowerCase() const {
    std::string result = data;
    for (char& c : result) {
        if (std::isupper(c)) {
            c = std::tolower(c);
        }
    }
    return ponystring(result);
}

// Функции удаления пробелов
ponystring ponystring::trim() const {
    size_t start = data.find_first_not_of(" \t\r\n");
    size_t end = data.find_last_not_of(" \t\r\n");
    if (start == std::string::npos || end == std::string::npos) {
        return ponystring(); // пустая строка, если все символы пробельные
    }
    return ponystring(data.substr(start, end - start + 1));
}


