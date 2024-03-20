#include "ponystring.h"
#include <vector>
#include <list>
#include <functional>

template<typename KeyType, typename ValueType>
class PPair {
private:
    // Внутренняя структура данных для хранения пар ключ-значение
    struct KeyValuePair {
        KeyType key;
        ValueType value;
        KeyValuePair(const KeyType& k, const ValueType& v) : key(k), value(v) {}
    };

    // Хэш-таблица для хранения элементов
    std::vector<List<KeyValuePair>> table;
    size_t capacity;

    // Хэш-функция для вычисления индекса в хэш-таблице
    size_t hash(const KeyType& key) const {
        // Простейшая хэш-функция - вычисляем остаток от деления ключа на размер хэш-таблицы
        return std::hash<KeyType>{}(key) % capacity;
    }

public:
    // Конструктор с параметрами для создания словаря с начальным списком пар ключ-значение
    PPair(std::initializer_list<std::pair<KeyType, ValueType>> initList, size_t initialCapacity = 10)
        : capacity(initialCapacity), table(initialCapacity) {
        for (const auto& pair : initList) {
            add(pair.first, pair.second);
        }
    }

    // Метод для добавления элемента в словарь
    void add(const KeyType& key, const ValueType& value) {
        size_t index = hash(key);
        table[index].push_back(KeyValuePair(key, value));
    }

    // Метод для получения значения по ключу
    ValueType get(const KeyType& key) const {
        size_t index = hash(key);
        for (const auto& pair : table[index]) {
            if (pair.key == key) {
                return pair.value;
            }
        }
        // Возвращаем значение по умолчанию, если ключ не найден
        return ValueType();
    }

    // Перегрузка оператора [] для доступа к значению по ключу
    ValueType& operator[](const KeyType& key) {
        size_t index = hash(key);
        for (auto& pair : table[index]) {
            if (pair.key == key) {
                return pair.value;
            }
        }
        // Если ключ не найден, создаем новую пару ключ-значение
        table[index].push_back(KeyValuePair(key, ValueType()));
        return table[index].back().value;
    }

    // Метод для удаления элемента по ключу
    void remove(const KeyType& key) {
        size_t index = hash(key);
        table[index].remove_if([&](const KeyValuePair& pair) { return pair.key == key; });
    }
};