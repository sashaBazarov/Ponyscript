from flask import *
from flask import Flask
from flask import request
import json
import db

app = Flask(__name__)
app.secret_key = "secret key"

@app.route('/downloadlib', methods=['GET', 'POST'])
def add_message():
    content = request.json
    try:
        package = db.get_package(content['name'])
    except IndexError:
        return "Package not found", 404
    
    print(package)

    answer = {'name': package[0], 'version': package[1], 'repository': package[2]}

    return jsonify(answer), 200

app.run(debug=True)