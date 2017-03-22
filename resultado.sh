#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Uso: ./resultado.sh pathDirectorioEntrada"
else
	for dir in `ls -v $1`
	do	
		rm -f $dir"_Result"
		echo "$dir" 	
		for entry in `ls -v "$1/$dir"`
		do
			result=$(./sin_proy "$1$dir/$entry")						
			y=(${result//$'\n'/ })
			echo ${y[0]} >> $dir"_Result"		 
		done
		python resultados.py $dir"_Opt" $dir"_Result"
		echo " "
	done
fi
