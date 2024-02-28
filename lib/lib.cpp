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

int sum(int a, int b){
    return a+b;
}

string replaceAll(const string& str, const string& oldSubstr, const string& newSubstr) {
    
    string result = str;
    size_t pos = 0;
    while((pos = result.find(oldSubstr, pos)) != string::npos) {
        result.replace(pos, oldSubstr.length(), newSubstr);
        pos += newSubstr.length();
    }
    return result;

}


void test_throw_ponyexception()
{
    throw PonyException("Oops! We have a problem with the friendsip!", FILE_INFO, __LINE__);
}


void neigh(string input){
    cout << input;
}

void neighln(string input){
    cout << input << endl;
}

string read(const string& text /*= ""*/){
    if (text != "") {
        cout << text;
    }
    string input;
    cin >> input;
    return input;
}

void readkey(){
    // if(text != ""){
    // cout << text;
    // }
    getch();
}

string str(int num){
    return to_string(num);
}

std::string str(double num){
    return to_string(num);
}


int to_int(std::string string){
    return stoi(string);
}











