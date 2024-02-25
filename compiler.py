from lexer import *
import os
import shutil
from pathlib import Path
import sys
import subprocess
import gccdocksparser
from logs import print_log, clear_log, print_error, log_path
from datetime import datetime
try:

    floader_dir = sys.argv[1]

    try:
        os.makedirs(floader_dir + "/logs")
    except: pass

    log_path = floader_dir + "/logs/" + datetime.now().strftime("%Y-%m-%d_%H-%M-%S") + ".log"

    clear_log()

    def extract_additional_files(file_path):
            with open(file_path, 'r') as file:
                additional_files = []
                start_tag_found = False

                for line in file:
                    if start_tag_found:
                        if line.strip() == '[Additional Files end]':
                            break
                        additional_files.append(line.strip())
                    elif line.strip() == '[Additional Files start]':
                        start_tag_found = True

                return additional_files

    print_log("Compiler started")

    try:
        os.makedirs(floader_dir + "/bin")
        print_log("Created bin directory")
    except FileExistsError:
        print_log("Bin directory already exists")
        pass

    bin_dir = floader_dir + "/bin"

    buildfiles = ""

    settings = {"filename":"",}
    
    print_log("Reading files in floader")

    print(os.listdir(floader_dir))

    variables = []
    classes = []

    for name in os.listdir(floader_dir):


        filename, fileext = os.path.splitext(name)

        compile = True

        if fileext == ".psc":
            fileext = ".cpp"
            
        elif fileext == ".psh":
            fileext = ".h"

        elif fileext == ".ponycfg":
            compile = False

        else:
            compile = False

        if compile:

            print_log("Compiling file " + name)

            with open(f"{floader_dir}/{name}", "r", encoding="utf-8") as f:
                lines = [line for line in f]
                code = "";
                for line in lines:
                    code = code + line

                with open(f"{bin_dir}/{filename}{fileext}", "w", encoding="utf-8") as file:
                    file.write("")
                    file.close()

                with open(f"{bin_dir}/{filename}{fileext}", "a", encoding="utf-8") as output:
                    output.write('#include <iostream>' + '\n')
                    output.write('#include "lib.h"' + '\n')
                    output.write(f'#define FILE_INFO {name}' + '\n')
                    output.write('using namespace std;' + '\n')

                    print_log("Lexical analysis")
                    tokens = lexical_analyzer(code)
                    print_log("Variable analysis")
                    for i in find_variables(tokens):
                        variables.append(i)
                    print_log("Class analysis")
                    classes_list, updated_tokens = find_classes(tokens)
                    for i in classes_list:
                        classes.append(i)
                    print_log("Translation")
                    tokens = translate_tokens(updated_tokens, variables, classes)
                    print_log("Assembling code")
                    assembled_code = assemble_code(tokens)
                    print_log("Writing code to file")
                    output.write(assembled_code)
                    print_log("Finished compiling file " + name)
            buildfiles = buildfiles + f"{bin_dir}/{filename}{fileext} "

    print_log("Reading configuration file")
    with open(f"{floader_dir}/.ponycfg") as cfg:
        additional_start = None
        additional_end = None
        for cfgline in cfg.readlines():
            if cfgline.split("=")[0].strip() == "[File Name]":
                settings["filename"] = cfgline.split("=")[1].strip()

    try:
        os.mkdir(f"{bin_dir}/lib")
    except FileExistsError:
        pass

    absp = os.path.abspath(__file__).replace("compiler.py", "")

    buildfiles = buildfiles + f"{bin_dir}/lib/lib.h "
    buildfiles = buildfiles + f"{bin_dir}/lib/lib.cpp "
    buildfiles = buildfiles + f"{bin_dir}/lib/file_manager.cpp "
    buildfiles = buildfiles + f"{bin_dir}/lib/ponyexceptions.h "
    buildfiles = buildfiles + f"{bin_dir}/lib/exceptions.cpp "
    buildfiles = buildfiles + f"{bin_dir}/lib/run.cpp "

    print_log("Copying additional files")
    shutil.copy(f"{absp}lib/lib.h", bin_dir + "/lib")
    shutil.copy(f"{absp}lib/lib.cpp", bin_dir  + "/lib")
    shutil.copy(f"{absp}lib/file_manager.cpp", bin_dir  + "/lib")
    shutil.copy(f"{absp}lib/ponyexceptions.h", bin_dir  + "/lib")
    shutil.copy(f"{absp}lib/exceptions.cpp", bin_dir  + "/lib")

    print_log("Building executable")
    print_log("Writing run.h")
    with open(f"{bin_dir}/lib/run.h", "w") as run:
        run.write(f'int magic(int argc, char *argv[]);')
    print_log("Writing run.cpp")
    shutil.copy(f"{absp}lib/run.cpp", bin_dir+"/lib")
    for file in extract_additional_files(f"{floader_dir}/.ponycfg"):
        buildfiles = buildfiles + file + " "

    print_log("Starting gcc compyler")
    result = subprocess.run(f"{absp}ucrt64\\bin\\g++.exe -o {bin_dir}/{settings['filename']} -I lib -finput-charset=UTF-8 {buildfiles}", capture_output=True)

    print_log(gccdocksparser.parse(result.stderr.decode()))

    if result.returncode == 0:
        print_log("\n")
        print_log("Compilation successful!") 
    else:
        raise Exception("Compilation failed!")

except Exception as e:
    print_log("\n")
    print_error(str(e))
    
exit()