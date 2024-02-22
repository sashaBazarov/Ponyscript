#include <iostream>
#include "lib.h"
#include "ponyexceptions.h"

using namespace std;

#define FILE_INFO std::string(replaceAll(__FILE__, ".cpp", ".psc"))

int magic(int argc, char *argv[])
{
throw PonyException("Oops! We have a problem with the friendsip!" , FILE_INFO, __LINE__); 
}
