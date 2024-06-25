import sys
import os

def read_file(path, essential=True):
    # Utilidad para asignar el valor de un archivo a una variable en un one-liner
	with open(path) as f:
		return f.read()

def write_file(fname, p, content):
	with open(os.path.join(p, fname), 'w') as f:
		f.write(content)

def get_path():
    # Obtener root de la aplicación
	if getattr(sys, 'frozen', False):
		return os.path.dirname(sys.executable) # Caso de ejecución .exe single file
	return os.path.abspath('.')

def write_config_file(name, path, immutableDict):
    # Toma un objeto immutableDict (formulario de HTML recibido por Flask)
    # Crea un archivo de configuración
    s = ''
    for i in immutableDict:
        s += i + '=' + immutableDict.get(i) + ','
    s = s.rstrip(',')
    write_file(name, path, s)

def read_config_file(path):
    # Lee un archivo de configuración
    # Devuelve un diccionario
    d = {}
    with open(path) as f:
        for line in f.read().split(','):
            (key, val) = line.split('=')
            d[key] = val
    return d