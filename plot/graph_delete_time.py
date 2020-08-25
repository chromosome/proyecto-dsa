
import numpy as np
import matplotlib.pyplot as plt
import os

#####################################################################
#####################################################################
##################         GRAPHS            ########################
#####################################################################
#####################################################################
#data = np.genfromtxt("depthsPerRegion_1800x3600.txt", delimiter=",")


#dataFile = open("citiesPerRegion_250x250.txt","r")
#dataFile = open("populationPerRegion_100x100.txt","r")
#dataFile = open("populationPerRegion_360x360.txt","r")
dataFile = open("deletion_time.txt","r")


times = []


for line in dataFile:
    words = line.split(",")
    times.append(float(words[0]))


dataFile.close()


plt.figure(1)
plt.plot(times)
plt.title("Tiempo promedio de eliminacion de nodos")
plt.xlabel("n")
plt.ylabel("tiempo [ms]")
plt.show()
plt.dpi(300)
#plt.savefig("depthsPerRegion", dpi=300)


#-----------------------------------------------


