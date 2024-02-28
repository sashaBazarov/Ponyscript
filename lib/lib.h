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

//Test functions
int sum(int a, int b);
void test_pony_exception();
//File fuctions
std::string readFile(const std::string& filename);

//Console functions
void neighln(std::string input);
void neigh(std::string input);

std::string read(const std::string& text = "");


void readkey();

//String functions
std::string replaceAll(const std::string& str, const std::string& oldSubstr, const std::string& newSubstr);

//Convert functions
std::string str(int num);
std::string str(double num);

int to_int(std::string string);

#endif