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
#include "ponystring.h"

#ifndef MYSTRINGFUNCTIONS_H  // Это директива препроцессора, чтобы избежать повторного включения файла
#define MYSTRINGFUNCTIONS_H


//Console functions
void neighln(ponystring input);
void neigh(ponystring input);

ponystring read();

void readkey();

//String functions

//Convert functions
ponystring str(double num);
ponystring str(int num);

int to_int(const ponystring& str);

#endif