import requests
from tqdm import tqdm
import zipfile
import io
import re
import os

absp = os.path.abspath(__file__).replace("derpy/derpy.py", "")

def get_repository_name_from_url(url):
    pattern = r'https?://github.com/([^/]+)/([^/]+)'

    match = re.match(pattern, url)
    
    if match:
        owner = match.group(1)
        repo_name = match.group(2)
        return owner, repo_name
    else:
        return False

def download_file(url, save_path = absp +"lib"):
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

    zip_ref.extractall(save_path + repository)

    zip_ref.close()


#https://api.github.com/repos/sashaBazarov/Ponyscript/releases
request = requests.post('http://127.0.0.1:5000/downloadlib', json={'name':'PonyScript', 'version': 'test3'})

if request.status_code == 200:
    print(request.json())
    package = request.json()
    author, repository = get_repository_name_from_url(package['repository'])

    request = requests.get(f'https://api.github.com/repos/{author}/{repository}/releases/latest')
    if request.status_code == 200:
        release = request.json()
        download_url = release['assets'][0]['browser_download_url']
        download_file(download_url)


