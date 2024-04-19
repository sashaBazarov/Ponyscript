from flask import *
from flask import Flask
from flask import request
import db
from flask import render_template
from flask_socketio import SocketIO
import re
import requests as req
import base64
import mistune
import githubapi


app = Flask(__name__)
app.secret_key = "secret key"
socketio = SocketIO(app)

def get_repository_name_from_url(url:str):
    pattern = r'https?://github.com/([^/]+)/([^/]+)'

    match = re.match(pattern, url)
    
    if match:
        owner = match.group(1)
        repo_name = match.group(2)
        return owner, repo_name
    else:
        return False

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/package', methods=['GET'])
def get_package():
    repository_url = db.get_package_url(request.args.get('name').strip())


    repository = githubapi.repository(repository_url)
    
    description = mistune.markdown(repository.readme)
    
    if repository.stars > 1000:
        stars:str = f"{round(repository.stars / 1000, 1)}k"
    else:
        stars:str = str(repository.stars)

    return render_template('package.html', description=description, name=repository.name, author=repository.author, stars=stars,  url=repository.url)

@app.route('/addlib', methods=['POST'])
def add_package():
    content = request.json
    db.add_package(content['name'], content['repository'])
    return "", 200

@app.route('/downloadlib', methods=['GET', 'POST'])
def add_message():
    content = request.json
    try:
        package = db.get_package(content['name'])
    except IndexError:
        return "Package not found", 404
    
    print(package)

    answer = {'name': package[0], 'repository': package[1]}

    return jsonify(answer), 200

@socketio.on('FindLibs')
def handle_message(name):
    socketio.emit('getlib', db.find_package(name))

socketio.run(app, debug=True, allow_unsafe_werkzeug=True)