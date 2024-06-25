#!/bin/bash

export PATH=.:$PATH

### Colors ##
ESC=$(printf '\033') RESET="${ESC}[0m" BLACK="${ESC}[30m" RED="${ESC}[31m"
GREEN="${ESC}[32m" YELLOW="${ESC}[33m" BLUE="${ESC}[34m" MAGENTA="${ESC}[35m"
CYAN="${ESC}[36m" WHITE="${ESC}[37m" DEFAULT="${ESC}[39m"

### Color Functions ##

greenprint() { printf "${GREEN}%s${RESET}\n" "$1"; }
blueprint() { printf "${BLUE}%s${RESET}\n" "$1"; }
redprint() { printf "${RED}%s${RESET}\n" "$1"; }
yellowprint() { printf "${YELLOW}%s${RESET}\n" "$1"; }
magentaprint() { printf "${MAGENTA}%s${RESET}\n" "$1"; }
cyanprint() { printf "${CYAN}%s${RESET}\n" "$1"; }

fn_vhas_gen() { clear; echo "Instalando version generica para VHAS..."; bash -l -f instalar.sh > install.log; echo "Version VHAS-Generica instalada correctamente."; sleep 3; mainmenu;}
fn_vhas2200_cf()  { clear; echo "Instalando version Carrefour para VHAS..."; bash -l -f instalar.carrefour12.sh > install.log; bash instalardep12.sh > install.log ; echo "Version VHAS-carrefour instalada correctamente."; sleep 3; mainmenu;}
fn_vhas2200_dia() { clear; echo "Instalando version Dia% para VHAS..."; bash -l -f instalar.dia12.sh > install.log; bash instalardep12.sh > install.log ; echo "Version VHAS-dia-porc instalada correctamente."; sleep 3; mainmenu;}
fn_vhas_kinderland() { clear; echo "Instalando version Kinderland para VHAS..."; bash -l -f instalar.kinderland.sh > install.log; bash instalardep12.sh > install.log ; echo "Version VHAS-kinderland instalada correctamente."; sleep 3; mainmenu;}


fn_do_borrarversion() { clear; echo "Borrando version actual..."; sleep 1; rm -rf ~/eprom ; rm ~/*.deb; rm ~/instalardep12.sh; sleep 1; echo "Version eliminada correctamente"; sleep 5; reboot; exit; }

fn_borrarversion() { 
    clear
    echo -ne "
$(blueprint 'Desea eliminar la version actual?')
$(redprint '1)') Si
$(redprint '2)') No
$(cyanprint '0)') Salir
Seleccione una opcion:
"
    read -r ans
    case $ans in
    1)
        fn_do_borrarversion
        ;;
    2)
        mainmenu
        ;;
    0)
        fn_bye
        ;;
    *)
        fn_fail
        ;;
    esac


    select yn in "Yes" "No"; do
    case $yn in
        Yes)
            rm -rf ~/eprom ; 
            rm ~/*.deb;
            rm ~/instalardep12.sh;
            mainmenu;
            ;;
        No) mainmenu;;
    esac
    done
   
    }

fn_bye() { echo "Adios!."; exit 0; }
fn_fail() { echo "Opcion erronea." exit 1; }

mainmenu() {
    clear
    echo -ne "
$(cyanprint '*************************')
$(cyanprint 'Instalacion Price Checker')
$(cyanprint '        Hasar Cia        ')
$(cyanprint '  Seleccione la version  ')
$(cyanprint '*************************')
$(greenprint '1)') Version Genérica V-HAS 2200/2300/2400
$(greenprint '2)') Version Carrefour V-HAS 2200/2300/2400 
$(greenprint '3)') Version Dia% V-HAS 2200/2300/2400
$(greenprint '4)') Version Kinderland V-HAS 2200/2300/2400
$(magentaprint '5)') Eliminar version actual
$(redprint '0)') Salir
Seleccione una opcion:  "
    read -r ans
    case $ans in
    1)
        fn_vhas_gen
        mainmenu
        ;;
    2)
        fn_vhas2200_cf
        mainmenu
        ;;
    3)
        fn_vhas2200_dia
        mainmenu
        ;;
    4)
    	fn_vhas_kinderland
    	mainmenu
    	;;
    5)
        fn_borrarversion
        ;;
    0)
        fn_bye
        ;;
    *)
        fn_fail
        ;;
    esac
}

mainmenu

