#include <fstream>
#include <string>

// Функция для чтения файла и возвращения его содержимого в виде строки
std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    std::string content;
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            content += line + "\n";
        }
        file.close();
        return content;
    } else {
        return ""; // Если не удалось открыть файл, возвращаем пустую строку
    }
}

// Функция для записи текста в файл
bool writeToFile(const std::string& filename, const std::string& content) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << content;
        file.close();
        return true; // Возвращаем true, если запись прошла успешно
    } else {
        return false; // Возвращаем false, если не удалось открыть файл для записи
    }
}
