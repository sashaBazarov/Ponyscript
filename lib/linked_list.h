#ifndef LIST_H
#define LIST_H

#include <iostream>
#include <sstream>
#include <initializer_list>
#include <stdexcept>

template<typename T>
class Node {
public:
    T data;
    Node<T>* next;

    Node(T data) : data(data), next(nullptr) {}
};

template<typename T>
class List {
private:
    Node<T>* head;
    Node<T>* tail;
    size_t size;

public:
    List() : head(nullptr), tail(nullptr), size(0) {}

    List(std::initializer_list<T> initList) : head(nullptr), tail(nullptr), size(0) {
        for (const auto& element : initList) {
            append(element);
        }
    }

    ~List() {
        clear();
    }

    void append(T data) {
        Node<T>* newNode = new Node<T>(data);
        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
        size++;
    }

    std::string toString() const {
        std::stringstream ss;
        Node<T>* current = head;
        while (current) {
            ss << current->data << " ";
            current = current->next;
        }
        return ss.str();
    }

    void sort() {
        if (!head || !head->next) return; // Если список пуст или содержит один элемент, то он уже отсортирован

        Node<T>* sorted = nullptr;
        Node<T>* current = head;

        while (current) {
            Node<T>* next = current->next;
            insertSorted(sorted, current);
            current = next;
        }

        head = sorted;
    }

    void insertSorted(Node<T>*& sorted, Node<T>* newNode) {
        if (!sorted || sorted->data >= newNode->data) {
            newNode->next = sorted;
            sorted = newNode;
        } else {
            Node<T>* current = sorted;
            while (current->next && current->next->data < newNode->data) {
                current = current->next;
            }
            newNode->next = current->next;
            current->next = newNode;
        }
    }

    void remove(const T& value) {
        Node<T>* current = head;
        Node<T>* prev = nullptr;

        while (current != nullptr) {
            if (current->data == value) {
                if (prev == nullptr) { // Если удаляемый элемент - первый в списке
                    head = current->next;
                } else {
                    prev->next = current->next;
                }

                if (current->next == nullptr) { // Если удаляемый элемент - последний в списке
                    tail = prev;
                }

                Node<T>* temp = current;
                current = current->next;
                delete temp;
                size--;
            } else {
                prev = current;
                current = current->next;
            }
        }
    }

    void remove_index(size_t index) {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }

        Node<T>* current = head;
        Node<T>* prev = nullptr;

        for (size_t i = 0; i < index; ++i) {
            prev = current;
            current = current->next;
        }

        if (prev == nullptr) {
            head = current->next;
        } else {
            prev->next = current->next;
        }

        if (current->next == nullptr) {
            tail = prev;
        }

        delete current;
        size--;
    }

    T& operator[](size_t index) {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }

        Node<T>* current = head;
        for (size_t i = 0; i < index; ++i) {
            current = current->next;
        }

        return current->data;
    }

    const T& operator[](size_t index) const {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }

        Node<T>* current = head;
        for (size_t i = 0; i < index; ++i) {
            current = current->next;
        }

        return current->data;
    }

    void clear() {
        while (head) {
            Node<T>* temp = head;
            head = head->next;
            delete temp;
        }
        size = 0;
    }

    size_t getSize() const {
        return size;
    }
};



#endif // LIST_H
