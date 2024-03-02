#include "sort.h"

template<typename T>
void bubbleSort(List<T>& list) {
    for (size_t i = 0; i < list.getSize() - 1; ++i) {
        for (size_t j = 0; j < list.getSize() - i - 1; ++j) {
            if (list[j] > list[j + 1]) {
                T temp = list[j];
                list[j] = list[j + 1];
                list[j + 1] = temp;
            }
        }
    }
}
template<typename T>
void insertionSort(List<T>& list) {
    for (size_t i = 1; i < list.getSize(); ++i) {
        T key = list[i];
        size_t j = i - 1;
        while (j >= 0 && list[j] > key) {
            list[j + 1] = list[j];
            --j;
        }
        list[j + 1] = key;
    }
}

