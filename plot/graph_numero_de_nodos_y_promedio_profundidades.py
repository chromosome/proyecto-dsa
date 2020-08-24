
import numpy as np
import matplotlib.pyplot as plt
import os

#####################################################################
#####################################################################
##################         GRAPHS            ########################
#####################################################################
#####################################################################


dataFile = open("profundidad_promedio_por_insert.txt","r")

black   = []
white   = []
grey    = []
med     = []


for line in dataFile:
    words = line.split(",")
    black.append(int(words[0]))
    white.append(int(words[1]))
    grey.append(int(words[2]))
    med.append(float(words[3]))

dataFile.close()

plt.figure(1)

plt.plot(black, label="nodos negros")
plt.plot(white, label="nodos blancos")
plt.plot(grey, label="nodos grises")
plt.title("Total de Nodos por Tipo")
plt.xlabel("Nodos Insertados")
plt.ylabel("Total de Nodos por Tipo")
plt.legend()
plt.show()

plt.figure(2)

plt.plot(med)
plt.title("Profundidad Promedio por Insert")
plt.xlabel("Nodos Insertados")
plt.ylabel("Profundidad Promedio")
plt.legend()
plt.show()



#-----------------------------------------------


