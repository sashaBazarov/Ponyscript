#include "lib.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "ponyexceptions.h"
#include <conio.h>

#define FILE_INFO std::string(__FILE__)
#define LINE_INFO std::to_string(__LINE__)

using namespace std;

void neigh(ponystring input){
    cout << input;
}

void neighln(ponystring input){
    cout << input << endl;
}

ponystring read() {
    std::string input;
    std::cin >> input;
    return ponystring(input);
}

void readkey(){
    // if(text != ""){
    // cout << text;
    // }
    getch();
}

ponystring str(int num) {
    return ponystring::fromInt(num);
}

ponystring str(double num) {
    return ponystring::fromDouble(num);
}

int to_int(const ponystring& str) {
    return std::stoi(str.str());
}

List<int> range(int start, int stop, int step) {
    List<int> list;
    for (int i = start; i < stop; i += step) {
        list.append(i);
    }
    return list;
}

