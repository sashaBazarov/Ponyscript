import re
from wordlist import wordlist
from logs import print_log

replacements = wordlist

variable_types = ['int', 'float', 'double', 'char', 'bool', 'string']

def find_variables(tokens):
    variables = []
    for i in range(0, len(tokens)):
        if tokens[i] in variable_types:
            j = 1
            while tokens[i + j] == '*':
                j += 1
            variables.append(tokens[i+j])

    return variables

def find_includes(tokens):
    includes = []
    for i in range(0, len(tokens)):
        if tokens[i] == 'friend':
            includes.append(tokens[i+1])
    return includes

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

    updated_tokens = tokens
    for i in insert_indexes:
        tokens.insert(i, '\n')
        tokens.insert(i+1, 'public:')
            

    return classes, tokens

def translate_tokens(tokens, variables, classes):
    includes = find_includes(tokens)
    for i in range(0, len(tokens)):
        try:
            if tokens[i+1] != '(' and tokens[i] not in variables and tokens[i] not in includes and tokens[i] != '\n' and tokens[i] not in ['+', '-', '*', '/', '=', '.', ',', ';', '(', ')', '[', ']', '{', '}'] and not tokens[i].startswith('"') and not tokens[i].endswith('"') and not tokens[i].isdigit() and tokens[i] not in classes:
                if tokens[i] in replacements:
                    tokens[i] = replacements[tokens[i]]
                else: 
                    raise Exception(f"Error: {tokens[i]} can`t be defined as command or object name")
        except IndexError: pass
    return tokens

def lexical_analyzer(code):
    tokens = re.findall(r'\<.*?\>|\b(?:int|#include|using|namespace|void|return|string|public|private|protected|byte|bool|neigh|read|class)\b|\w+|".*?"|[.,!?;(){}\[\]=+\-*/]|[\n]', code)
    tokens = ["#include" if item == "include" else item for item in tokens]
    return tokens

def assemble_code(tokens):
    assembled_code = ''
    for token in tokens:
        if token == '\n':
            assembled_code += '\n'
        elif token.startswith('"') and token.endswith('"'):
            assembled_code += ' ' + token
        elif assembled_code.endswith('\n'):
            assembled_code += token
        elif token in ['+', '-', '*', '/', '=', '.', ',', ';', '(', ')', '[', ']', '{', '}']:
            assembled_code += token
        elif token == 'using':
            assembled_code += token
        else:
            assembled_code += ' ' + token
    return assembled_code


