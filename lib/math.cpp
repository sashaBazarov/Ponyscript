#include <iostream>


constexpr double PI = 3.14159265358979323846;
// Функция для вычисления факториала
int factorial(int n) {
    if (n == 0)
        return 1;
    else
        return n * factorial(n - 1);
}

// Функция для вычисления степени числа
double power(double base, int exponent) {
    double result = 1.0;
    for (int i = 0; i < exponent; ++i)
        result *= base;
    return result;
}

// Функция для вычисления синуса
double sin(double x) {
    double result = 0.0;
    for (int n = 0; n <= 10; ++n) {
        result += power(-1, n) * power(x, 2 * n + 1) / factorial(2 * n + 1);
    }
    return result;
}

// Функция для вычисления косинуса
double cos(double x) {
    double result = 0.0;
    for (int n = 0; n <= 10; ++n) {
        result += power(-1, n) * power(x, 2 * n) / factorial(2 * n);
    }
    return result;
}

// Функция для вычисления тангенса
double tan(double x) {
    return sin(x) / cos(x);
}

// Функция для вычисления котангенса
double ctan(double x) {
    return cos(x) / sin(x);
}

// Функция для вычисления натурального логарифма
double log(double x) {
    if (x <= 0) {
        return 0.0; // Лучше вернуть NaN или другое специальное значение
    }
    double result = 0.0;
    double term = (x - 1) / (x + 1);
    for (int n = 0; n < 100; ++n) {
        result += power(term, 2 * n + 1) / (2 * n + 1);
    }
    return 2 * result;
}

double radians(double angle) {
    return angle * 3.14159265358979323846 / 180.0;
}
