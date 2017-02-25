import sys
file_opt = open(sys.argv[1], 'r')
file_result = open(sys.argv[2], 'r')
optimo = []
resultados = []
desviacion = [] 
desviacionTotal = 0.0

for line in file_opt:
	optimo.append(float(line))
file_opt.close()

for line in file_result:
	resultados.append(float(line))
file_result.close()

name = sys.argv[1].split("_")[0]
name += "_Desv"
file_desv = open(name, 'w')
for i in range(0,len(optimo)):
	if (optimo[i] != 0):
		desviacion.append(100 * ((optimo[i] - resultados [i]) / optimo[i]))
		desviacionTotal += desviacion[i]
	else:
		desviacion.append(100 * (optimo[i] - resultados [i]))
		#desviacionTotal += desviacion[i]
		file_desv.write("Optimo Cero: ")
	file_desv.write(str(desviacion[i]) + "\n")

file_desv.write("Desviacion Total: " + str(desviacionTotal))
file_desv.close()
print("Desviacion Total: " + str(desviacionTotal)) 