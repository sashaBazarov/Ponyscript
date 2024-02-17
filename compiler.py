from lexer import analizetext
import os
import shutil
from pathlib import Path

floader_dir = ""
with open(".ponytemp") as pt:
    floader_dir = pt.readlines()[0]

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
                for line in lines:
                    #print(analizetext(line.strip()))
                    output.write(analizetext(line.strip()) + "\n")
            
        buildfiles = buildfiles + f"{bin_dir}/{filename}{fileext} "



with open(f"{floader_dir}/.ponycfg") as cfg:
    for cfgline in cfg.readlines():
        if cfgline.split("=")[0].strip() == "[File Name]":
            settings["filename"] = cfgline.split("=")[1].strip()

try:
    os.mkdir(f"{bin_dir}/lib")
except FileExistsError:
    pass

absp = os.path.abspath(__file__).replace("compiler.py", "")



shutil.copy2(f"{absp}lib/lib.cpp", f"{bin_dir}")
shutil.copy2(f"{absp}lib/lib.h", f"{bin_dir}")

buildfiles = buildfiles + f"{bin_dir}/lib.h "
buildfiles = buildfiles + f"{bin_dir}/lib.cpp "


os.system(f"g++ -o {settings['filename']} {buildfiles}")

try:
    os.remove(bin_dir+settings["filename"]+".exe")
except:pass

shutil.move(settings["filename"]+".exe", bin_dir)

#os.system(f"{os.path.dirname(__file__)}\\bin\\pony.exe")
print("build complete")
exit()
