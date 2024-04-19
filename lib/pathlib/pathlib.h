#include <string>
#include "ponystring.h"
#include <filesystem>

namespace pathlib{

bool make_directory(ponystring path);

bool remove_directory(ponystring path);

bool deleteAllFilesInDirectory(const ponystring& directory);

bool copyFile(const ponystring& source, const ponystring& destination);

bool moveFile(const ponystring& source, const ponystring& destination);

bool isFile(const ponystring& filePath);

}