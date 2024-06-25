#!/bin/bash

export PATH=.:$PATH

echo ""
echo "Instalacion Price Checker"
echo " Carrefour"
echo " Hasar Cia. "
echo ""

cp ./pchecker.carrefour12.tgz ~/
cp ./libxdo3_13a3.20130111.1-3.1_armhf.deb ~/
cp ./xdotool_13a3.20130111.1-3.1_armhf.deb ~/
cp ./firefox_28.0+build2-0ubuntu2_armhf.deb ~/
cp ./xul-ext-ubufox_2.8-0ubuntu1_all.deb ~/
cp ./instalardep12.sh ~/

#Vamos al home
cd ~/

chmod +x instalardep12.sh

#Descomprimimos el archivo
tar xvfz pchecker.carrefour12.tgz .

rm ./pchecker.carrefour12.tgz

cd ~/eprom

chmod +x startup
chmod +x startpricechk
chmod +x ambiente
chmod +x getplu.carrefour
chmod +x server

cd ~/
sudo chown -R ubuntu eprom

echo ""
echo "Instalacion completada."
echo ""

# Exit from the script with success (0)
exit 0

