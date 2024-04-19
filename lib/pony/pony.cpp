#include <cstdlib>
#include "ponystring.h"
#include <pthread.h>
#include <filesystem>
#include <string>
#include <direct.h>

#ifdef _WIN32
    #include <windows.h>
    #include <tchar.h>
#elif __linux__
    #include <sys/sysinfo.h>
#else
    #error Unsupported operating system
#endif

namespace System{

    int execute(ponystring command){
        return std::system(command.str().c_str());
    }

    ponystring get_current_dir(){

        std::filesystem::path currentPath = std::filesystem::current_path();
        std::string executableName = "executable_name"; 
        std::filesystem::path executablePath = currentPath / executableName;
        return executablePath.parent_path().string();

    }    

    ponystring os()
    {

        #ifdef _WIN64
        return "Windows 64-bit";
        #elif   _WIN32
        return "Windows 32-bit";
        #elif __APPLE__ || __MACH__
        return "Mac OSX";
        #elif __linux__
        return "Linux";
        #elif __FreeBSD__
        return "FreeBSD";
        #elif __unix || __unix__
        return "Unix";
        #else
        return "Other";
        #endif
  
    }

    
    int getAvailableRAM() {
    #ifdef _WIN32 || _WIN64
        MEMORYSTATUSEX status;
        status.dwLength = sizeof(status);
        GlobalMemoryStatusEx(&status);
        return static_cast<int>(status.ullAvailPhys);
    #elif __linux__
        struct sysinfo memInfo;
        sysinfo(&memInfo);
        return static_cast<int>(memInfo.freeram * memInfo.mem_unit);
    #else
        #error Unsupported operating system
    #endif
    }

    int getTotalRAM() {
        #ifdef _WIN32
            MEMORYSTATUSEX status;
            status.dwLength = sizeof(status);
            GlobalMemoryStatusEx(&status);
            return static_cast<int>(status.ullTotalPhys / (1024 * 1024)); // Возвращаем значение в гигабайтах
        #elif __linux__
            struct sysinfo memInfo;
            sysinfo(&memInfo);
            return static_cast<int>((memInfo.totalram * memInfo.mem_unit) / (1024 * 1024 * 1024)); // Возвращаем значение в гигабайтах
        #else
            #error Unsupported operating system
        #endif
    }

ponystring getCPUName() {
    #ifdef _WIN32 || _WIN64
        std::string cpuName = "Unknown";
        SYSTEM_INFO sysInfo;
        GetSystemInfo(&sysInfo);
        if (sysInfo.dwProcessorType == PROCESSOR_INTEL_386 || sysInfo.dwProcessorType == PROCESSOR_AMD_X8664) {
            HKEY hKey;
            LONG lResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &hKey);
            if (lResult == ERROR_SUCCESS) {
                char processorName[255];
                DWORD bufSize = sizeof(processorName);
                lResult = RegQueryValueExA(hKey, "ProcessorNameString", NULL, NULL, reinterpret_cast<LPBYTE>(processorName), &bufSize);
                if (lResult == ERROR_SUCCESS) {
                    cpuName = processorName;
                }
                RegCloseKey(hKey);
            }
        }
        return cpuName;

    #elif __linux__

    std::ifstream cpuinfo("/proc/cpuinfo");
    std::string line;
    while (std::getline(cpuinfo, line)) {
        if (line.substr(0, 6) == "model ") {
            return line.substr(line.find(":") + 2);
        }
    }
    return "Unknown";
    
    #endif


}


#ifdef _WIN32 || _WIN64
ponystring readRegistryValue(HKEY hKey, const std::string& subKey, const std::string& valueName) {
    std::string valueData;
    HKEY hSubKey;
    LONG result = RegOpenKeyExA(hKey, subKey.c_str(), 0, KEY_READ, &hSubKey);
    if (result == ERROR_SUCCESS) {
        DWORD valueType;
        DWORD valueSize;
        result = RegQueryValueExA(hSubKey, valueName.c_str(), NULL, &valueType, NULL, &valueSize);
        if (result == ERROR_SUCCESS) {
            if (valueType == REG_SZ || valueType == REG_EXPAND_SZ || valueType == REG_MULTI_SZ) {
                char* buffer = new char[valueSize];
                result = RegQueryValueExA(hSubKey, valueName.c_str(), NULL, NULL, reinterpret_cast<LPBYTE>(buffer), &valueSize);
                if (result == ERROR_SUCCESS) {
                    valueData = buffer;
                }
                delete[] buffer;
            } else if (valueType == REG_DWORD || valueType == REG_QWORD) {
                DWORD dwordValue;
                result = RegQueryValueExA(hSubKey, valueName.c_str(), NULL, NULL, reinterpret_cast<LPBYTE>(&dwordValue), &valueSize);
                if (result == ERROR_SUCCESS) {
                    valueData = std::to_string(dwordValue);
                }
            }
        }
        RegCloseKey(hSubKey);
    }
    return valueData;
}
#endif
}
        

