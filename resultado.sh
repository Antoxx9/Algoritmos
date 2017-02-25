#!/bin/bash

if [ $# -ne 2 ]; then
	echo "Uso: ./resultado.sh pathDirectorioEntrada nameFileSalida"
else
	for entry in `ls -v $1`
	do
		result=$(./sin_proy < "$1$entry")
		echo "$result"
		echo " "
		y=(${result//$'\n'/ })
		echo ${y[0]} >> $2		 
	done
fi

