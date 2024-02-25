import re
import os

def find_new_functions(code):
    # Regular expression pattern to match function declarations
    pattern = r'(?<!\w)(?:void|int|char|float|double|string|bool)\s+\w+\s*\([^)]*\)\s*{'
    matches = re.findall(pattern, code)

    out = []
    if matches:

        for match in matches:
            try:
                out.append(match.replace("{", "").split("\n")[0] + ";")
            except:
                out.append(match.replace("{", "") + ";")
    
    return out


import re

def find_new_classes(filename):
    """
    Find and print all new class declarations in a .cpp file.
    """
    if not filename.endswith('.cpp'):
        print("Error: Input file must be a .cpp file.")
        return

    if not os.path.exists(filename):
        print(f"Error: File {filename} does not exist.")
        return

    with open(filename, 'r') as file:
        content = file.read()

    # Regular expression pattern to match class declarations
    pattern = r'\bclass\s+\w+\s*(?::{0,2}\s*(?:public|private|protected)?\s*\w+)?\s*{'
    matches = re.findall(pattern, content)

    if matches:
        print("New class declarations found:")
        for match in matches:
            print(match)
    else:
        print("No new class declarations found.")

if __name__ == "__main__":
    filename = input("Enter filename for .cpp file: ")
    find_new_classes(filename)


# if __name__ == "__main__":
#     print(*find_new_functions("""
# #include <iostream>
# #include "lib.h"
# #define FILE_INFO prog.psc
# using namespace std;
#  #include <locale.h>

# int magic( int argc, char* argv[])
# {
# setlocale( LC_ALL, "ru_.utf-8");

# string a= read( "First num: ");
# string b= read( "Second num: ");
# neighln( "Sum equal "+ to_string( stoi( a)+ stoi( b)));
# neigh( "Press any key to continue...");
# readkey();
# return 0;
# }
# """))
