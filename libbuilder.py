
import os

absp = os.path.abspath(__file__).replace("libbuilder.py", "")

from wordlist import wordlist

import re

import libparser

variable_types = ['int', 'float', 'double', 'char', 'bool', 'string']
list_types = ['List', 'PPair']

replacements = wordlist

libs, psclibs = libparser.parcse_libs()

def get_libs(tokens):
    includes = []
    for i in range(0, len(tokens)):
        if tokens[i] == 'friend':
            if tokens[i+1] in libs:
                includes.append(tokens[i+1])
            elif tokens[i+1] in psclibs:
                includes.append(tokens[i+1])


    return includes

def find_variables(tokens, classes):
    variables = {}
    for i in range(0, len(tokens)):
        if tokens[i] in variable_types:
            j = 1
            while tokens[i + j] == '*':
                j += 1
            variables[tokens[i+j]] = tokens[i]

        elif tokens[i] in list_types:
            j = 1
            if tokens[i+1].startswith('<') and tokens[i+1].endswith('>'):
                string = tokens[i+1].strip('<').strip('>') 
                
                j += 1
            while tokens[i + j] == '*':
                j += 1
            variables[tokens[i+j]] = tokens[i]
        
        elif tokens[i] in classes:
            j = 1
            while tokens[i + j] == '*':
                j += 1
            variables[tokens[i+j]] = tokens[i]

    return variables

def find_classes(tokens:list):
    classes = []
    insert_indexes = []
    for i in range(0, len(tokens)-1):
        if tokens[i] == 'class':
            classes.append(tokens[i+1])
            j=0
            while tokens[i+j] != "{":
                j+=1
                if tokens[i+j] == '{':
                    insert_indexes.append(i+j+1) 

    # updated_tokens = tokens
    # for i in insert_indexes:
    #     tokens.insert(i, '\n')
    #     tokens.insert(i+1, 'public:')
            

    return classes, tokens


def find_namespaces(tokens:list):
    namespaces = []
    insert_indexes = []
    for i in range(0, len(tokens)-1):
        if tokens[i] == 'namespace':
            namespaces.append(tokens[i+1])
            j=0
            while tokens[i+j] != "{":
                j+=1
                if tokens[i+j] == '{':
                    insert_indexes.append(i+j+1)


    return namespaces

def format_strings(tokens):
    for i in range(0, len(tokens)):
        if tokens[i-1] == 'f' and tokens[i].startswith('"') and tokens[i].endswith('"'):
            string_tokens = re.findall(r'\<.*?\>|\w+|{.*?}|[.,!?;:()\[\]=+\-*/<>]|', tokens[i].strip('"'))
            string = 'ponystring("")'
            for token in string_tokens:
                if token.startswith('{') and token.endswith('}'):
                    string += '+ ' + token.strip('{').strip('}')
                else:
                    string += '+ ' + f'ponystring("{token} ")'

            tokens[i] = string
            tokens[i-1] = ''
    return tokens      

def translate_tokens(tokens, variables, classes:list, namespaces:list) -> list:
    includes = get_libs(tokens)
    for i in range(0, len(tokens)):
        try:
            if tokens[i] in variables:
                pass
            else:
                if tokens[i] == 'f' and tokens[i+1].startswith('"') and tokens[i+1].endswith('"'):
                    pass
                else:
                    if tokens[i+1] != '(' and tokens[i] not in variables and tokens[i] not in list_types and tokens[i] not in includes and tokens[i] != '\n' and tokens[i] not in ['+', '-', '*', '/', '=', '.', ',', ';', '(', ')', '[', ']', '{', '}', ':', '<', '>'] and not tokens[i].startswith('"') and not tokens[i].endswith('"') and not tokens[i].isdigit() and tokens[i] not in classes and not tokens[i].startswith('<') and not tokens[i].endswith('>') and tokens[i] not in namespaces:
                        if tokens[i] in replacements:
                            tokens[i] = replacements[tokens[i]]
                        else: 
                            raise Exception(f"Error: {tokens[i]} can`t be defined as command or object name")

        except IndexError: pass

    for i in range(0, len(tokens)):
        if tokens[i] in classes and tokens[i+1] == '.':
            tokens[i+1] = '::'
        if tokens[i] in namespaces and tokens[i+1] == '.':
            tokens[i+1] = '::'
    return tokens

def lexical_analyzer(code):
    tokens = re.findall(r'\<.*?\>|\b(?:int|#include|using|namespace|void|return|string|public|private|protected|byte|bool|neigh|read|class)\b|\w+|".*?"|[.,!?;:(){}\[\]=+\-*/<>]|[\n]', code)
    tokens = ["#include" if item == "include" else item for item in tokens]
    return tokens


def assemble_code(tokens):
    assembled_code = ''
    last_token = ''
    for token in tokens:

        if token == '\n':
            assembled_code += '\n'
        elif token.startswith('"') and token.endswith('"') and last_token != '#include':
            assembled_code += ' ' + f'ponystring({token})'
        elif assembled_code.endswith('\n'):
            assembled_code += token
        elif token in ['+', '-', '*', '/', '=', '.', ',', ';', '(', ')', '[', ']', '{', '}', ':', '<', '>']:
            assembled_code += token
        elif token == 'using':
            assembled_code += token
        else:
            if token.isdigit() and last_token == '.':
                assembled_code += token
            else:
                assembled_code += ' ' + token
        last_token = token
    return assembled_code


def build_lib(libname):
    i = libname
    lib_tokens = lexical_analyzer(open(f"{absp}lib/{i.strip()}/{i.strip()}.psc", "r").read())
    lib_classes = find_classes(lib_tokens)[0]
    lib_variables = find_variables(lib_tokens, lib_classes)
    lib_namespaces = find_namespaces(lib_tokens)
    tokens = translate_tokens(lib_tokens, lib_variables, lib_classes, lib_namespaces)
    tokens = format_strings(tokens)
    assembled_code = assemble_code(tokens)
    with open(f"{absp}lib/{i.strip()}/{i.strip()}.cpp", "w") as file:
        file.write(assembled_code)

    lib_tokens = lexical_analyzer(open(f"{absp}lib/{i.strip()}/{i.strip()}.psh", "r").read())
    lib_classes = find_classes(lib_tokens)[0]
    lib_variables = find_variables(lib_tokens, lib_classes)
    lib_namespaces = find_namespaces(lib_tokens)
    tokens = translate_tokens(lib_tokens, lib_variables, lib_classes, lib_namespaces)
    tokens = format_strings(tokens)
    assembled_code = assemble_code(tokens)
    with open(f"{absp}lib/{i.strip()}/{i.strip()}.h", "w") as file:
        file.write(assembled_code)