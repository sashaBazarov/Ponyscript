import re

def extract_functions_constants_classes(file_path):
    functions = []
    constants = []
    classes = []
    includes = set()

    with open(file_path, 'r') as file:
        lines = file.readlines()

        in_comment_block = False
        current_function = ""
        current_class = ""

        for line in lines:
            # Check for comment blocks
            if '/*' in line:
                in_comment_block = True
            if '*/' in line:
                in_comment_block = False
                continue
            
            if not in_comment_block:
                # Extract functions
                function_match = re.match(r'^\s*([\w:<>]+)\s+([\w:<>]+)\s*\((.*)\)\s*{', line)
                if function_match:
                    return_type = function_match.group(1)
                    function_name = function_match.group(2)
                    arguments = function_match.group(3)
                    functions.append((return_type, function_name, arguments))
                
                # Extract constants
                constant_match = re.match(r'^\s*const\s+([\w:<>]+)\s+([\w]+)\s*=\s*([^;]+);', line)
                if constant_match:
                    constant_type = constant_match.group(1)
                    constant_name = constant_match.group(2)
                    constant_value = constant_match.group(3)
                    constants.append((constant_type, constant_name, constant_value))
                
                # Extract classes
                class_match = re.match(r'^\s*class\s+([\w:<>]+)', line)
                if class_match:
                    class_name = class_match.group(1)
                    classes.append(class_name)

                # Extract includes
                include_match = re.match(r'^\s*#include\s+<(.*)>', line)
                if include_match:
                    includes.add(include_match.group(1))

    return functions, constants, classes, includes

def write_to_header(header_path, functions, constants, classes, includes):
    with open(header_path, 'w') as header_file:
        for include in includes:
            header_file.write(f'#include <{include}>\n')

        header_file.write('\n')

        for constant in constants:
            header_file.write(f'constexpr {constant[0]} {constant[1]} = {constant[2]};\n')

        if constants:
            header_file.write('\n')

        for function in functions:
            header_file.write(f'{function[0]} {function[1]}({function[2]});\n')

        if functions:
            header_file.write('\n')

        for class_name in classes:
            header_file.write(f'class {class_name};\n')


def generate_header(cpp_file_path, header_file_path):
    """
    This fuction generates  header file from a C++ file.
    
    cpp_file_path: The path to the C++ file.
    header_file_path: The path to the generated header file.
    
    Returns: None
    
    Example usage:
    generate_header('lib/math.cpp', 'example.h')
    
    This will generate a header file 'example.h' from the C++ file 'lib/math.cpp'.
    The generated header file will contain the functions, constants, and classes defined in the C++ file.
    The header file will also contain the includes defined in the C++ file.
    
    The header file will be written to the specified path.
    
    The header file will contain the following syntax:
    
    #include <math.h>
    
    constexpr double PI = 3.14159265358979323846;
    
    double add(double a, double b);
    double subtract(double a, double b);

    """
    functions, constants, classes, includes = extract_functions_constants_classes(cpp_file_path)
    write_to_header(header_file_path, functions, constants, classes, includes)