import requests
from tqdm import tqdm
import zipfile
import io
import re
import os
import sys
import shutil

absp = os.path.abspath(__file__).replace("derpy\derpy.py", "")

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


def get_repository_name_from_url(url):
    pattern = r'https?://github.com/([^/]+)/([^/]+)'

    match = re.match(pattern, url)
    
    if match:
        owner = match.group(1)
        repo_name = match.group(2)
        return owner, repo_name
    else:
        return False

def download_file(url, repository_name ,save_path = absp +"lib"):
    # Отправляем GET-запрос для получения данных файла
    response = requests.get(url, stream=True)
    # Получаем размер файла из заголовков ответа
    file_size = int(response.headers.get('content-length', 0))
    
    # Используем tqdm для отображения прогресс-бара
    progress_bar = tqdm(total=file_size, unit='B', unit_scale=True)

    zip_buffer = io.BytesIO()

    for data in response.iter_content(chunk_size=1024):
        # Записываем данные в файл
        zip_buffer.write(data)
        # Обновляем значение прогресс-бара
        progress_bar.update(len(data))
    
    # Закрываем прогресс-бар
    progress_bar.close()

    zip_buffer.seek(0)

    zip_ref = zipfile.ZipFile(zip_buffer, 'r')

    zip_ref.extractall(save_path + "/" + repository_name)

    zip_ref.close()


print(absp)



if sys.argv[1] == "instal":
    #https://api.github.com/repos/sashaBazarov/Ponyscript/releases
    request = requests.post('http://127.0.0.1:5000/downloadlib', json={'name':sys.argv[2], 'version': 'test3'})

    if request.status_code == 200:
        print(request.json())
        package = request.json()
        author, repository = get_repository_name_from_url(package['repository'])

        request = requests.get(f'https://api.github.com/repos/{author}/{repository}/releases/latest')
        if request.status_code == 200:
            release = request.json()
            download_url = release['assets'][0]['browser_download_url']
            download_file(download_url, repository)

        if request.status_code == 404:
            print(f"Repository {sys.argv[2]} not found")

elif sys.argv[1] == "remove":
    shutil.rmtree(absp + "lib/" + sys.argv[2])
    print("Lib removed")

elif sys.argv[1] == "list":
    for i in parcse_libs():
        print(i)

elif sys.argv[1] == "update":
    for i in parcse_libs():
        if i != sys.argv[2]:
            shutil.rmtree(absp + "lib/" + i)
    request = requests.post('https://api.github.com/repos/', json={'name':sys.argv[2], 'version': 'test3'})

    if request.status_code == 200:
        print(request.json())
        package = request.json()
        author, repository = get_repository_name_from_url(package['repository'])

        request = requests.get(f'https://api.github.com/repos/{author}/{repository}/releases/latest')
        if request.status_code == 200:
            release = request.json()
            download_url = release['assets'][0]['browser_download_url']
            download_file(download_url, repository)

        if request.status_code == 404:
            print(f"Repository {sys.argv[2]} not found")

elif sys.argv[1] == "publish":
    name = sys.argv[2]
    reposytory = sys.argv[3]
    if re.match(r'^[a-zA-Z0-9_]+$', name) and re.match(r'^https?://github.com/([^/]+)/([^/]+)', reposytory):
        request = requests.post('http://127.0.0.1:5000/addlib', json={'name':name, 'repository': reposytory})

        if request.status_code == 200:
            print(request.json())

        else:
            print("Invalid repository or name")

elif sys.argv[1] == "-version":
    print("0.0.1")
    print("Author: sanya_fritz")
    print("Repository: https://github.com/sashaBazarov/Ponyscript")
    print("License: MIT")
    print("Description: Derpy is a simple command-line tool for downloading and installing libraries from GitHub.")
    print("Usage: derpy [command] [library name]")
    print("Commands:")
    print("  -version: Display the version number and other information")
    print("  list: List all installed libraries")
    print("  instal: Install a library from GitHub")
    print("  remove: Remove an installed library")
    print("  update: Update an installed library")
    print("  help: Display this help message")
    print("Examples:")
    print("  derpy -version")
    print("  derpy list")
    print("  derpy instal [repository name]")
    print("  derpy remove [repository name]")
    print("  derpy update [repository name]")

elif sys.argv[1] == "-help":
    print("Derpy is a simple command-line tool for downloading and installing libraries from GitHub.")
    print("Usage: derpy [command] [library name]")
    print("Commands:")
    print("  -version: Display the version number and other information")
    print("  list: List all installed libraries")
    print("  instal: Install a library from GitHub")
    print("  remove: Remove an installed library")
    print("  update: Update an installed library")
    print("  help: Display this help message")
    print("Examples:")
    print("  derpy -version")
    print("  derpy list")
    print("  derpy instal [repository name]")
    print("  derpy remove [repository name]")
    print("  derpy update [repository name]")
    print("  derpy -help")


else:
    print("Invalid command")


