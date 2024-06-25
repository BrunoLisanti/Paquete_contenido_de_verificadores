import socket
import sys

import configparser, os

import warnings

warnings.filterwarnings("ignore", category=DeprecationWarning)

if len(sys.argv) < 2:
    print("clientpc nroplu")
    exit()

msg = 'Articulo no encontrado'

plu = sys.argv[1]

if len(plu) == 12:
    plu += '0'

req = b'' + plu.encode('ascii') + b"\r\n"

config = configparser.ConfigParser()

config.readfp(open('config.ini'))
server=config.get("connection", "server")

port=config.get("connection", "port")

#try:
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((server, int(port)))
s.send(req)
data = s.recv(1024)
s.close()

data = data.decode('utf-8')

if (len(data.split('\x1b')) == 10):
    Descripcion=data.split('\x1b')[7].strip('\x03').strip('.4')
    final_column = data.split('\x1b')[9]
    final_column = final_column.split('\x03')
    final_column = final_column[0].lstrip('.7').split('$')
    Price = final_column[1][:len(final_column[1].split('.')[0]) + 3]
    combo_quantity = final_column[1][len(Price):]
    combo_price = final_column[2]
    msg = 'Codigo=' + plu + '|Descripcion=' + Descripcion + '|PrecioUnit=' + Price + '|OfertaCantidad=' + combo_quantity + "|OfertaPrecio=" + combo_price

print(msg)

#except Exception as e:
    #print("Articulo no encontrado")
