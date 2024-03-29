import sqlite3


def get_package(name) -> list:
    connection = sqlite3.connect('packages.db')
    cursor = connection.cursor()

    cursor.execute('SELECT `name`, `version`, `repository`  FROM `packages` WHERE `name` = ?', (name,))

    package = cursor.fetchall()

    connection.close()
    
    return package[0]


def add_package(name, version, repository):
    connection = sqlite3.connect('packages.db')
    cursor = connection.cursor()

    cursor.execute('INSERT INTO `packages` (`name`, `version`, `repository`) VALUES (?, ?, ?)', (name, version, repository))

    connection.commit()
    connection.close()


