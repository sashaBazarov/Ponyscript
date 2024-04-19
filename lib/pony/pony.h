#include "ponystring.h"

#ifdef _WIN32 || _WIN64
#include <windows.h>
#endif

namespace System{
    int execute(ponystring command);

    ponystring get_current_dir();

    ponystring os();

    int getAvailableRAM();

    int getTotalRAM();

    ponystring getCPUName();


    #ifdef _WIN32 || _WIN64
    ponystring readRegistryValue(HKEY hKey, const std::string& subKey, const std::string& valueName);
    #endif

}