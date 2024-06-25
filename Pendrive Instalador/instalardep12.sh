#!/bin/bash

echo ""
echo "Instalando XDOTOOL."
echo " Hasar Cia. "
echo ""

cd ~

dpkg -i libxdo3_13a3.20130111.1-3.1_armhf.deb

dpkg -i xdotool_13a3.20130111.1-3.1_armhf.deb

dpkg -i firefox_28.0+build2-0ubuntu2_armhf.deb

dpkg -i xul-ext-ubufox_2.8-0ubuntu1_all.deb
 
rm libxdo3_13a3.20130111.1-3.1_armhf.deb

rm xdotool_13a3.20130111.1-3.1_armhf.deb

rm firefox_28.0+build2-0ubuntu2_armhf.deb

rm xul-ext-ubufox_2.8-0ubuntu1_all.deb

rm instalardep12.sh

reboot
