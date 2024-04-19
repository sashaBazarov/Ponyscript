import sqlite3



def get_package(name) -> list:
    try:
        connection = sqlite3.connect('packages.db')
        cursor = connection.cursor()

        cursor.execute('SELECT `name`, `repository`  FROM `packages` WHERE `name` = ?', (name,))

        package = cursor.fetchall()

        connection.close()
        
        return package[0]
    except:
        return []
    

def get_package_url(name) -> list:
    try:
        connection = sqlite3.connect('packages.db')
        cursor = connection.cursor()

        cursor.execute('SELECT `repository`  FROM `packages` WHERE `name` = ?', (name,))

        package = cursor.fetchall()

        connection.close()
        
        return package[0][0]
    except:
        return []

def add_package(name, repository) -> bool:
    try:
        connection = sqlite3.connect('packages.db')
        cursor = connection.cursor()

        cursor.execute('INSERT INTO `packages` (`name`, `repository`) VALUES (?, ?)', (name, repository))

        connection.commit()
        connection.close()
        return True
    except:
        return False

def find_package(name) -> list:
    try:
        connection = sqlite3.connect('packages.db')
        cursor = connection.cursor()

        cursor.execute(f'''SELECT `name`, `version` FROM `packages` WHERE `name` LIKE '%{name}%' ''')

        package = cursor.fetchall()

        

        connection.close()

        return package
    except: 
        return []


