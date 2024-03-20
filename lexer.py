import re
from lib.wordlist import wordlist
from lib.constants import constants
from lib.libs import libs
from lib.classes import buildintclasses
replacements = wordlist

variable_types = ['int', 'float', 'double', 'char', 'bool', 'string']
list_types = ['List', 'PPair']


def find_variables(tokens):
    variables = {}
    for i in range(0, len(tokens)):
        if tokens[i] in variable_types:
            j = 1
            while tokens[i + j] == '*':
                j += 1
            variables[tokens[i+j]] = tokens[i]

        elif tokens[i] in list_types:
            j = 2
            while tokens[i + j] == '*':
                j += 1
            variables[tokens[i+j]] = tokens[i]

    return variables

def get_libs(tokens):
    includes = []
    for i in range(0, len(tokens)):
        if tokens[i] == 'friend':
            if tokens[i+1] in libs:
                includes.append(tokens[i+1])
    return includes


def find_includes(tokens):
    includes = []
    for i in range(0, len(tokens)):
        if tokens[i] == 'friend':
            if tokens[i+1] in libs:
                tokens[i+1] = f'"{libs[tokens[i+1]]}.h"'
            else: tokens[i+1] = f'"{tokens[i+1]}.h"'
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

    # updated_tokens = tokens
    # for i in insert_indexes:
    #     tokens.insert(i, '\n')
    #     tokens.insert(i+1, 'public:')
            

    return classes, tokens

def translate_tokens(tokens, variables, classes:list):
    for i in buildintclasses:
        classes.append(i)
    includes = find_includes(tokens)
    for i in range(0, len(tokens)):
        try:
            if tokens[i] in variables:
                print(tokens[i])
            else:
                if tokens[i+1] != '(' and tokens[i] not in variables and tokens[i] not in list_types and tokens[i] not in includes and tokens[i] != '\n' and tokens[i] not in ['+', '-', '*', '/', '=', '.', ',', ';', '(', ')', '[', ']', '{', '}', ':', '<', '>'] and not tokens[i].startswith('"') and not tokens[i].endswith('"') and not tokens[i].isdigit() and tokens[i] not in classes and tokens[i] not in constants and not tokens[i].startswith('<') and not tokens[i].endswith('>'):
                    if tokens[i] in replacements:
                        tokens[i] = replacements[tokens[i]]
                    else: 
                        raise Exception(f"Error: {tokens[i]} can`t be defined as command or object name")
        except IndexError: pass
    return tokens

def merge_float_tokens(tokens):
    merged_tokens = []
    i = 0
    while i < len(tokens):
        if re.match(r'\d+\.\d+', tokens[i]):
            # Начинаем объединение с текущего токена числа с плавающей точкой
            merged_token = tokens[i]
            i += 1
            # Продолжаем объединение, пока следующий токен также является числом с плавающей точкой
            while i < len(tokens) and re.match(r'\d+\.\d+', tokens[i]):
                merged_token += tokens[i]
                i += 1
            # Добавляем объединенный токен числа с плавающей точкой
            merged_tokens.append(merged_token)
        else:
            # Другие токены добавляем без изменений
            merged_tokens.append(tokens[i])
            i += 1
    return merged_tokens

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
        elif token.startswith('"') and token.endswith('"'):
            assembled_code += ' ' + token
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


def remove_comments(tokens):
    code = assemble_code(tokens)
    # Удаление однострочных комментариев
    code = re.sub(r'//.*', '', code)
    
    # Удаление многострочных комментариев
    code = re.sub(r'/\*(.*?)\*/', '', code, flags=re.DOTALL)
    
    return lexical_analyzer(code)

