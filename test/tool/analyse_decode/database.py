import sqlite3
from os import path

def open_db():
    db_path = path.join("test", "context", "data", "analisis_decode.sqlite")
    init_script_path = path.join("test", "tool", "analyse_decode", "sql_scripts", "init.sql")
    with open(init_script_path, 'r') as f: init_script = f.read()
    connection = sqlite3.connect(db_path)
    cursor = connection.cursor()
    cursor.execute(init_script)
    connection.commit()
    return connection

def close_db(connection):
    connection.close()