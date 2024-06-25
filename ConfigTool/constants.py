from utils import get_path, read_file
from os import path

# Nombres de archivos
SERVE_FOLDER= "www" # Carpeta que tendra los mismos archivos que el www que contiene el verificador
LOGO_FNAME	= "logo" 
DB_CFG_FNAME= "db.cfg" 
UI_CFG_FNAME= "ui.cfg"
# Paths
APP_PATH 	= get_path()
SERVE_ROOT	= path.join(APP_PATH, SERVE_FOLDER)
RESOURCES	= path.join(SERVE_ROOT, 'resources')
EPROM		= path.join(SERVE_ROOT, 'eprom')
# Símbolos utilizados en el verificador
STYLES		= ["original", "prueba", "cuadricula"] # Nombres de los archivos de estilos en el verificador. Usado para ver si se está escribiendo un archivo ui.cfg válido.
COLUMN_NAMES= {"CODE": "Codigo", "DESC": "Descripcion", "PRICE": "PrecioUnit", "PROMOTION": "Oferta"} # Nombres de columnas que se manejan en el HTML tanto de este servidor (en form, query) como el del verificador (en plu.asc, en index.html). Estas son las que se escriben en la consulta SQL para identificar cada columna con un nombre definido.
# Servidor
PORT        = read_file(path.join(APP_PATH, 'puerto.txt')).strip()