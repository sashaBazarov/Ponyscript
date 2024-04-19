import requests
import re
import base64

def get_repository_name_from_url(url:str):
    pattern = r'https?://github.com/([^/]+)/([^/]+)'

    match = re.match(pattern, url)
    
    if match:
        owner = match.group(1)
        repo_name = match.group(2)
        return owner, repo_name
    else:
        return False

class repository:
    def __init__(self, url:str):
        self.url = url

        self.author, self.name = get_repository_name_from_url(self.url)
        

        self.stars = len(requests.get(f'https://api.github.com/repos/{self.author}/{self.name}/stargazers').json())


        self.readme = base64.b64decode(requests.get(f'https://api.github.com/repos/{self.author}/{self.name}/readme').json()['content']).decode('utf-8')