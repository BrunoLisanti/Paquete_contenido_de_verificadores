from flask import Flask, send_from_directory, request
from os import path, listdir, remove
import filecmp
import shutil
import pyodbc
import utils
from constants import *

db_conn	= None
app 	= Flask(__name__, static_url_path='', static_folder=SERVE_ROOT)

@app.route("/")
def interface():
	# Levanta todos los contenidos a partir de la carpeta www
	return send_from_directory(SERVE_ROOT, 'index.html')

@app.route("/Logo")
def logo_listing():
	# Devuelve el listado de archivos en 'logos' 
	# Usado desde Javascript para encontrar y mostrar los logos disponibles
	return listdir(path.join(RESOURCES, "logos"))

@app.route("/Logo/Upload", methods=["POST"])
def logo_upload():
	# Subir imagen a la carpeta logos
	# Los archivos no se sobreescriben
	f = request.files["file"]
	save_dir = path.join(RESOURCES, "logos")

	if len(listdir(save_dir)) >= 10:
		return "Solo puede subir hasta 10 logos"

	f.filename = LOGO_FNAME
	duplicates = 0
	while (path.exists(path.join(save_dir, f.filename))):
		f.filename = LOGO_FNAME + str(duplicates)
		duplicates += 1

	f.save(path.join(save_dir, f.filename))

	return "Imagen subida satisfactoriamente."

@app.route("/Logo/Set/", methods=["POST"])
def logo_set():
	# Copiar el logo seleccionado a eprom
	logo_file = request.data.decode("UTF-8")
	if not path.isfile(path.join(RESOURCES, "logos", logo_file)):
		return "El archivo de logo '" + logo_file + "' no existe."
	
	shutil.copy(path.join(RESOURCES, "logos", logo_file), path.join(EPROM, 'www', 'images', 'Logo_Custom.png'))
	return "Logo actualizado correctamente."

@app.route("/Logo/Delete/", methods=["DELETE"])
def logo_delete():
	# Eliminar logo de la lista para elegir
	logo_file = request.data.decode("UTF-8")
	if not path.isfile(path.join(RESOURCES, "logos", logo_file)):
		return "El archivo de logo '" + logo_file + "' no existe."
	
	if (path.isfile(path.join(EPROM, 'www', 'images', 'Logo_Custom.png'))):
		if (filecmp.cmp(path.join(RESOURCES, "logos", logo_file), path.join(EPROM, 'www', 'images', 'Logo_Custom.png'))):
			remove(path.join(EPROM, 'www', 'images', 'Logo_Custom.png'))
	
	remove(path.join(RESOURCES, "logos", logo_file))
	return "Logo removido correctamente"

@app.route("/UI/Set/", methods=["POST"])
# TODO: falta implementar
def style_set():
	# Escribir archivo cfg de interfaz en eprom
	if request.form.get("name") not in STYLES: 
		return "El estilo no es válido."
	
	utils.write_config_file(UI_CFG_FNAME, EPROM, request.form)

	# Cambiar preview de acuerdo a la nueva configuración
	shutil.copy(path.join(EPROM, "www", "views", request.form.get("html")), path.join(EPROM, "www", "index.html"))
	shutil.copy(path.join(EPROM, "www", "views", request.form.get("css")), 	path.join(EPROM, "www", "index.css"))

	return "Archivo actualizado correctamente."

@app.route("/DB/ModeConfigUpload", methods=["POST"])
def db_mode_config_upload():
	# Cambiar si los datos se acceden desde el plu.asc o la base de datos sql
	utils.write_config_file("getplu.cfg", path.join(EPROM), request.form)
	
	return "Modo actualizado correctamente."

@app.route("/DB/ConfigUpload", methods=["POST"])
def db_config_upload():
	# Escribir archivo cfg de BD en www
	utils.write_config_file(DB_CFG_FNAME, path.join(SERVE_ROOT), request.form)
	return "Configuración de base de datos actualizada correctamente."

@app.route("/getplu/<plu>")
def get_precio(plu):
	# TODO: probar qué pasa al editar dsn manualmente (se vuelve None?)
	global db_conn
	if db_conn == None:
		try:
			dsn 	= utils.read_config_file(path.join(SERVE_ROOT, DB_CFG_FNAME)).get("dsn")
			db_conn = pyodbc.connect("DSN=" + dsn)
		except Exception as err:
			return str(err)
		

	# Genera el string de consulta a la BD SQL leyendo el archivo de configuración de BD
	user_columns = utils.read_config_file(path.join(SERVE_ROOT, DB_CFG_FNAME))
	
	query = 'SELECT '
	for column_name in COLUMN_NAMES.values():
		user_column = user_columns.get(column_name)
		if user_column is not None:
			query += user_column + " as " + column_name + ','
	query = query.rstrip(',')
	query += " FROM " + user_columns.get("Tabla") + " WHERE " + user_columns.get("Codigo") + " = " + plu
			
	cursor = db_conn.cursor()
	try:
		cursor.execute(query)
	except Exception as err:
		return "Problema en la consulta SQL: " + str(err)

	result = cursor.fetchone()

	if result is not None:
		columnNames = [column[0] for column in cursor.description]
		plu_Line = ""
		for i in range(len(columnNames)):
			plu_Line += columnNames[i] + "=" + str(result[i]) + "|"
		plu_Line = plu_Line.rstrip('|')

		return plu_Line
	
	return "Artículo no encontrado"

if __name__  == '__main__':
	app.run(host="0.0.0.0", port=PORT)