import re
import os
from colorama import init, Fore

default_additional_lines = 4

def extract_file_paths(text):
    # Regular expression to match file paths
    filepath_regex = r'[a-zA-Z]:?[^:\n]+:\d+:\d+'
    file_paths = re.findall(filepath_regex, text)
    return file_paths


def parse_file_line(line):
    # Define the regex pattern to match the file path, line number, and character number
    pattern = r'^(.*?):(\d+):(\d+)$'
    
    # Match the regex pattern against the input line
    match = re.match(pattern, line)
    
    if match:
        # Extract path, line number, and character number from the matched groups
        file_path = match.group(1)
        line_number = int(match.group(2))
        char_number = int(match.group(3))
        
        return file_path, line_number, char_number
    else:
        return None


def parse(logs):
    out = """"""
    lines = logs.split('\n')
    a:list = []
    for line in lines:
        if line != "":
            a.append(extract_file_paths(line))
        a = list(filter(None, a))

    for i in a:
        path, ln, let = parse_file_line(i[0])
        if path.split('/')[-1] != "run.cpp":
            if path.split('.')[-1] == "cpp":
                path = path.replace(path.split('.')[-1], "psc")
            if path.split('.')[-1] == "h":
                path = path.replace(path.split('.')[-1], "psh")
            
            for line in lines:
                for token in line.split(':'):
                    if token.strip() == 'error':
                        line = Fore.RED + line + Fore.RESET + '\n'

                if i[0] in line:
                    out += '\n' + line.replace(i[0], f"File: {path.split('/')[-1]} | Line: {int(ln)-default_additional_lines} \n").split("\n")[0] + '\n' + line.replace(i[0], f"File: {path.split('/')[-1]} | Line: {int(ln)-3} \n").split("\n")[1][1:].strip()

    return out