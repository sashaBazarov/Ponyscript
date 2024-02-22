from lexer import analizetext
import os
import shutil
from pathlib import Path
import sys

try:

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

    floader_dir = sys.argv[1]
    # with open(".ponytemp") as pt:
    #     floader_dir = pt.readlines()[0]

    try:
        os.makedirs(floader_dir + "/bin")
    except FileExistsError:
        pass

    bin_dir = floader_dir + "/bin"

    buildfiles = ""



    settings = {"filename":"",}

    print(os.listdir(floader_dir))


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
            print(name)

            with open(f"{floader_dir}/{name}", "r") as f:
                lines = [line for line in f]

                with open(f"{bin_dir}/{filename}{fileext}", "w") as file:
                    file.write("")
                    file.close()

                with open(f"{bin_dir}/{filename}{fileext}", "a") as output:
                    output.write('#include <iostream>' + '\n')
                    output.write('#include "lib.h"' + '\n')
                    output.write(f'#define FILE_INFO {name}' + '\n')
                    for line in lines:
                        #print(analizetext(line.strip()))
                        output.write(analizetext(line.strip()) + "\n")
                
            buildfiles = buildfiles + f"{bin_dir}/{filename}{fileext} "




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



    # shutil.copy2(f"{absp}lib/lib.cpp", f"{bin_dir}")
    # shutil.copy2(f"{absp}lib/lib.h", f"{bin_dir}")

    buildfiles = buildfiles + f"{bin_dir}/lib.h "
    buildfiles = buildfiles + f"{bin_dir}/lib.cpp "
    buildfiles = buildfiles + f"{bin_dir}/file_manager.cpp "
    buildfiles = buildfiles + f"{bin_dir}/ponyexceptions.h "
    buildfiles = buildfiles + f"{bin_dir}/exceptions.cpp "
    buildfiles = buildfiles + f"{bin_dir}/run.cpp "

    print(absp)

    shutil.copy(f"{absp}lib/lib.h", bin_dir)
    shutil.copy(f"{absp}lib/lib.cpp", bin_dir)
    shutil.copy(f"{absp}lib/file_manager.cpp", bin_dir)
    shutil.copy(f"{absp}lib/ponyexceptions.h", bin_dir)
    shutil.copy(f"{absp}lib/exceptions.cpp", bin_dir)

    shutil.copy(f"{absp}lib/lib.h", bin_dir + "/lib")
    shutil.copy(f"{absp}lib/lib.cpp", bin_dir  + "/lib")
    shutil.copy(f"{absp}lib/file_manager.cpp", bin_dir  + "/lib")
    shutil.copy(f"{absp}lib/ponyexceptions.h", bin_dir  + "/lib")
    shutil.copy(f"{absp}lib/exceptions.cpp", bin_dir  + "/lib")


    with open(f"{bin_dir}/run.h", "w") as run:
        run.write(f'int magic(int argc, char *argv[]);')

    shutil.copy(f"{absp}lib/run.cpp", bin_dir)

    for file in extract_additional_files(f"{floader_dir}/.ponycfg"):
        buildfiles = buildfiles + file + " "


    print(f"{absp}ucrt64/bin/g++.exe -o {settings['filename']} {buildfiles}")

    os.system(f"{absp}ucrt64/bin/g++.exe -o {settings['filename']} {buildfiles}")

    try:
        os.remove(bin_dir+settings["filename"]+".exe")
    except:pass

    try:
        shutil.move(settings["filename"]+".exe", bin_dir)
    except:
        try:
            os.remove(bin_dir+ "/" + settings["filename"]+".exe")
        except:pass
        shutil.move(settings["filename"]+".exe", bin_dir)



    #os.system(f"{os.path.dirname(__file__)}\\bin\\pony.exe")
    print("build complete")

except Exception as e:
    print("build failed")
    print(e)
    

try:
    os.remove(bin_dir+'/lib.h')
    os.remove(bin_dir+'/lib.cpp')
    os.remove(bin_dir+'/file_manager.cpp')
    os.remove(bin_dir+'/ponyexceptions.h')
    os.remove(bin_dir+'/exceptions.cpp')
    os.remove(bin_dir+'/run.cpp')
    os.remove(bin_dir+'/run.h')
except:pass

exit()
