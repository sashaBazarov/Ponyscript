#include "lib.h"
#include <iostream>
#include "run.h"
#include "ponyexceptions.h"

int main(int argc, char *argv[]) {
try
{
    return magic(argc, argv);

}
catch(const std::exception& e)
{
    std::cerr << e.what() << '\n';
}
catch (const PonyException& e) 
{
    std::cerr << "Exception: " << e.what() << "\nFile: " << e.getFile() << "\nLine: " << e.getLine() << std::endl;
}


}