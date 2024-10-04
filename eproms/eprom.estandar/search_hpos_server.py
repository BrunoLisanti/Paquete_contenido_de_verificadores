import socket
import select
from subprocess import Popen, PIPE
import time
from shutil import move

def get_broadcast_addresses():
	output = Popen(["./get_broadcast_addresses"],stdout=PIPE)
	response = output.communicate()
	return response[0].split("\n")

token = b"\x95"
udp_port = 9594

while True:
	broadcast_addresses = list(get_broadcast_addresses())
	if broadcast_addresses.count == 0:
		raise Exception("no broadcast addresses available")
	for broadcast_address in broadcast_addresses:
		try:
			sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
			sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
			sock.setblocking(0)
			sock.sendto(token, (broadcast_address, udp_port))
			timeout = 2 # seconds
			ready = select.select([sock], [], [], timeout)
			if ready[0]:
				data, addr = sock.recvfrom(1)
				if data == token and addr[0]:
					with open("_hpos_server.txt", "w") as f:
						f.write(addr[0])
						f.close()
						move("_hpos_server.txt", "hpos_server.txt")
					time.sleep(5)
		except Exception as err:
            #print(broadcast_address + f": {err=}, {type(err)=}")
            print(broadcast_address + ": err={}, type(err)={}".format(err, type(err)))
			continue
	time.sleep(1)
