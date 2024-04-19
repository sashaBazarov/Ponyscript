import os
absp = os.path.abspath(__file__).replace("libparser.py", "")

def list_directories(path):
    directories = []
    
    if os.path.isdir(path):
        items = os.listdir(path)
        for item in items:
            item_path = os.path.join(path, item)
            if os.path.isdir(item_path):
                directories.append(item)
    return directories



def parcse_libs():
    libs = []
    psclibs = []
    for i in list_directories(f"{absp}lib"):
        if i != "dlls":
            if os.path.isfile(f"{absp}lib/{i}/libinfo"):
                if os.path.isfile(f"{absp}lib/{i}/{i}.psc"):
                    psclibs.append(i)
                else: libs.append(i)



                
    return libs, psclibs




print(parcse_libs())

