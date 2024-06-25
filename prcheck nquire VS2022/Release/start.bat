@echo off
REM ######################################################################
REM # Nombre: Start.bat
REM # Descripcion: Levanta el proceso prcheck
REM ######################################################################

prcheck.exe -p:3334 -x:c:\data\plu.asc
:FIN
exit

