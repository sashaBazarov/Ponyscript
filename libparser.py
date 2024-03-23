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
    for i in list_directories(f"{absp}lib"):
        if i != "dlls":
            if os.path.isfile(f"{absp}lib/{i}/libinfo"):
                libs.append(i)
                
    return libs

