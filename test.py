import re

def extract_functions_constants_classes(file_path):
    functions = []
    constants = []
    classes = []

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
                constant_match = re.match(r'^\s*constexpr\s+([\w:<>]+)\s+([\w]+)\s*=\s*([^;]+);', line)
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

    return functions, constants, classes

def write_to_header(header_path, functions, constants, classes):
    with open(header_path, 'w') as header_file:
        for constant in constants:
            header_file.write(f'constexpr {constant[0]} {constant[1]} = {constant[2]};\n')

        for function in functions:
            header_file.write(f'{function[0]} {function[1]}({function[2]});\n')

        for class_name in classes:
            header_file.write(f'class {class_name};\n')

# Example usage
cpp_file_path = 'lib/math.cpp'
header_file_path = 'example.h'

functions, constants, classes = extract_functions_constants_classes(cpp_file_path)
write_to_header(header_file_path, functions, constants, classes)
