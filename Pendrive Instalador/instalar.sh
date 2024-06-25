#!/bin/bash

export PATH=.:$PATH

echo ""
echo "Instalacion Price Checker"
echo " Hasar Cia. "
echo ""


cp ./pchecker.tgz ~/
cp ./libxdo3_13a3.20130111.1-3.1_armhf.deb ~/
cp ./xdotool_13a3.20130111.1-3.1_armhf.deb ~/
cp ./instalardep.sh ~/

#Vamos al home
cd ~/

chmod +x instalardep.sh

#Descomprimimos el archivo
tar xvfz pchecker.tgz 

rm ./pchecker.tgz

cd ~/eprom

#chmod +x startup
chmod +x startpricechk
#chmod +x ambiente
chmod +x getplu
chmod +x server
chmod +x server_conn
chmod +x change_ui

cd ~/
sudo chown -R ubuntu eprom

echo ""
echo "Instalacion completada."
echo ""

# Exit from the script with success (0)
exit 0

