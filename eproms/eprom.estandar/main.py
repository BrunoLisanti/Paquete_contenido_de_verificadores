import requests
import sys
import time
import json

def get_server_location():
	while True:
		try:
			with open('hpos_server.txt') as f:
				return f.readline().replace('\n', '')
		except:
			time.sleep(1)

def get_database_server(ip):
	url = "http://" + ip + ":8080/nodes"
	response = requests.get(url)
	node = response.json()
	return node["hostBaseDatos"], node["hostOfertas"] + node["recursoOfertas"]

try:
	hosts = get_database_server("localhost")
	database_host = hosts[0]
	promotions_host = hosts[1]
except:
	server = get_server_location()
	# dbhost = get_database_server(server)
	database_host = "http://" + server + ":8081"
	promotions_host = "http://" + server + ":8082/promotions" # bold assumption

session = requests.session()
session.headers =	{
						"Accept": "application/json",
						"Content-Type": "application/json"
					}
nplu = sys.argv[1]
url = database_host + "/database/plu/" + nplu
response = session.get(url)
plu = response.json()

try:
	data = {
				"pedidoOfertasPorPLU":
				{
					"hostBaseDatos": database_host,
					"soloOfertasDelDia": True,
					"incluirPromocionesGlobales": False,
					"codigo": nplu
				}
			}
	response = session.post(promotions_host, json.dumps(data))
	rc = response.json()
	promotions = ""
	for promotion in rc:
		if len(promotions):
			promotions += "_"
		if "textoPromocion" in promotion:
			if (len(promotion["textoPromocion"])):
				promotions += promotion["textoPromocion"]
			else:
				promotions += promotion["descripcion"]
		else:
			promotions += promotion["descripcion"]
except Exception as err:
	promotions = ""

print("Codigo=" + plu["id"] + "|Descripcion=" + plu["descripcion"] + "|PrecioUnit=" + str(plu["precio"]) + "|Oferta=" + promotions)
