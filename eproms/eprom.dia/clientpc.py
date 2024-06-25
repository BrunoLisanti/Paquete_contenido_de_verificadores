import requests
import sys

import ConfigParser, os
from requests.auth import HTTPBasicAuth
import json

if len(sys.argv) < 2:
    print("client nroplu")
    exit()

plu = sys.argv[1]

config = ConfigParser.ConfigParser()

config.readfp(open('config.ini'))

url=config.get("connection", "url")
usuario=config.get("basicauth","usuario")
clave=config.get("basicauth","clave")
tienda=config.get("general","tienda")

payload="{\"Tienda\": " + tienda + ", \"Ean\": " + plu + " }"

headers = {
  'User': usuario,
  'Password': clave,
  'Content-Type': 'application/json'
}

try:
    response = requests.request("POST", url, headers=headers, data=payload, verify=False)

    data  = json.loads(response.content)

    if data["sdtPrcChkr"]["ArticuloSuceso"]:
        msg = 'Codigo=' + plu + '|Descripcion=' + data["sdtPrcChkr"]["ArticuloDsc"].encode('utf-8') + '|PrecioUnit=' + str(data["sdtPrcChkr"]["ArticuloPvp"].replace(".",",")) + '|PrecioUnitF=' + str(data["sdtPrcChkr"]["ArticuloPvpf"].replace(".",",")) + '|Ofertas=' + data["sdtPrcChkr"]["ArticuloInf"].encode('utf-8') + '|ProdUrl=' + data["sdtPrcChkr"]["ArticuloImgURI"].encode('utf-8')
    else:
        msg = "Articulo no encontrado"

    print(msg)

except:
    print("Articulo no encontrado")