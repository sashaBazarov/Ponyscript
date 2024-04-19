from lexer import *
import os
import shutil
from pathlib import Path
import sys
import subprocess
import gccdocksparser
from logs import *
from datetime import datetime
from headergenerator import *

try:
    absp = os.path.abspath(__file__).replace("compiler.py", "")

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

    variables = {}
    classes = []
    includes = []
    namespaces = []

    for name in os.listdir(floader_dir):

        filename, fileext = os.path.splitext(name)

        compile = True

        if fileext == ".psc":
            fileext = ".cpp"
            
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
                    output.write(f'#include "{absp}lib/lib.h"' + '\n')
                    output.write(f'#include "{absp}lib\linked_list.h"' + '\n')
                    output.write(f'#define FILE_INFO {name}' + '\n')
                    #output.write('using namespace std;' + '\n')

                    print_log("Lexical analysis")
                    tokens = lexical_analyzer(code)

                    tokens = remove_comments(tokens)

                    print(tokens)

                    for i in get_libs(tokens):
                        if i not in includes:
                            includes.append(i)

                    

                    print_log("Variable analysis")
                    for i in find_variables(tokens):
                        variables[i] = find_variables(tokens)[i]
                    print(variables)
                    

                    for i in includes:
                        temptokens = lexical_analyzer(open(f"{absp}lib/{i.strip()}/{i.strip()}.h", "r").read())
                        temptokens = remove_comments(temptokens)
                        tempvariables = find_variables(temptokens)
                        for j in tempvariables:
                            variables[j] = tempvariables[j]

                    
                    
                    print_log("Class analysis")
                    classes_list, updated_tokens = find_classes(tokens)
                    for i in classes_list:
                        classes.append(i)

                    for i in includes:
                        tempclasses, temptokens = find_classes(remove_comments(lexical_analyzer(open(f"{absp}lib/{i.strip()}/{i.strip()}.h", "r").read())))
                        for j in tempclasses:
                            classes.append(j)

                    print_log("Namespace analysys")
                    for i in find_namespaces(tokens):
                        namespaces.append(i)
                    
                    for i in includes:
                        tempnamespaces = find_namespaces(remove_comments(lexical_analyzer(open(f"{absp}lib/{i.strip()}/{i.strip()}.h", "r").read())))
                        for j in tempnamespaces:
                            namespaces.append(j)

                    print_log("Translation")

                    tokens = translate_tokens(updated_tokens, variables, classes, namespaces, includes)

                    tokens = format_strings(tokens)
                    
                    print_log("Assembling code")
                    assembled_code = assemble_code(tokens)
                    print_log("Writing code to file")
                    output.write(assembled_code)
                    print_log("Finished compiling file " + name)

            generate_header(f"{bin_dir}/{filename}{fileext}", f"{bin_dir}/{filename}.h")
            buildfiles = buildfiles + f"{bin_dir}/{filename}{fileext} "
            buildfiles = buildfiles + f"{bin_dir}/{filename}.h "

    print_log("Reading configuration file")
    with open(f"{floader_dir}/.ponycfg") as cfg:
        additional_start = None
        additional_end = None
        for cfgline in cfg.readlines():
            if cfgline.split("=")[0].strip() == "[File Name]":
                settings["filename"] = cfgline.split("=")[1].strip()



    lib = "/lib" #Эта фигня нужна для дебага, gcc переодически теряет файлы в каталого lib. "/lib" - значение для каталога lib
    if lib != "":
        try:
            shutil.rmtree(f"{bin_dir}{lib}") #Удаляем каталог lib, если он существует
        except: pass
        try:
            os.mkdir(f"{bin_dir}{lib}")
        except FileExistsError:
            pass

    print_log("Copying built in libriaries")

    # buildfiles = buildfiles + f"{bin_dir}/lib/file_manager.cpp "

    buildfiles = buildfiles + f"{bin_dir}{lib}/run.cpp "

    
    # buildfiles = buildfiles + f"{bin_dir}/lib/math.cpp "

    print_log("Copying additional files")

    print_log("Copying additional built-in libriaries")
    for i in includes:

        shutil.copy(f"{absp}lib/{i.strip()}/{i.strip()}.cpp", bin_dir + lib)
        shutil.copy(f"{absp}lib/{i.strip()}/{i.strip()}.h", bin_dir + lib)
        buildfiles = buildfiles + f"{bin_dir}{lib}/{i}.cpp "
        buildfiles = buildfiles + f"{bin_dir}{lib}/{i}.h "

    print_log("Building executable")
    print_log("Writing run.h")

    with open(f"{bin_dir}{lib}/run.h", "w") as run:
        run.write(f'int magic(int argc, char *argv[]);')


    print_log("Writing run.cpp")

    with open(f"{bin_dir}{lib}/run.cpp", "w") as run:

        content = """"""

        content = content + f'#include "{absp}lib\linked_list.h"' + '\n'
        content = content + f'#include "{absp}lib\lib.h"' + '\n'
        content = content + f'#include "run.h"' + '\n'
        content = content + f'#include "{absp}lib\ponyexceptions.h"' + '\n' 

        content = content + """

            int main(int argc, char *argv[]) {
            try
            {
                return magic(argc, argv);

            }
            catch(const std::exception& e)
            {
                std::cerr << e.what();
            }
            catch (const PonyException& e) 
            {
                std::cerr << "Exception: " << e.what() << "File: " << e.getFile() << "Line: " << e.getLine() << std::endl;
            }


            }
            """
        run.write(content)
    
    # shutil.copy(f"{absp}lib/run.cpp", bin_dir+ lib)


    print_log("Starting gcc compyler")


    objectfiles = ""
    for i in buildfiles.split():

        if os.path.basename(i).split('.')[-1] == 'cpp':
            print_log("Compiling " + i)
            result = subprocess.run(f"{absp}ucrt64\\bin\\g++.exe -c  {i} -o {bin_dir}/{os.path.basename(i).split('.')[0]}.o -I{absp}lib -I{bin_dir}/lib", capture_output=True)
            objectfiles = objectfiles + f"{bin_dir}/{os.path.basename(i).split('.')[0]}.o "
            if result.returncode != 0:
                print(gccdocksparser.parse(result.stderr.decode()))
                raise Exception("Compilation failed!")


    print(f"{absp}ucrt64\\bin\\g++.exe -o {bin_dir}/{settings['filename']} {objectfiles} -L{absp}lib -I{absp}lib -lponylib -finput-charset=UTF-8 -lstdc++ ")
    compiler = subprocess.run(f"{absp}ucrt64\\bin\\g++.exe -o {bin_dir}/{settings['filename']} {objectfiles} -I{absp}lib -I{bin_dir}/lib -L{absp}lib  -lponylib -finput-charset=UTF-8 -lstdc++ ", capture_output=True)

    # for i in os.listdir(bin_dir):
    #     if i.split('.')[-1] == 'o':
    #         os.remove(bin_dir + "/" + i)

    if compiler.returncode == 0:
        print_log("Compilation successful!") 
    else:
        print_log(compiler.stderr.decode())
        raise Exception("Compilation failed!")

except Exception as e:
    print_log("\n")
    print_error(str(e))
    
exit()