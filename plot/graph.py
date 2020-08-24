
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
dataFile = open("depthsPerRegion_1800x3600.txt","r")



x = []
y = []
z = []

for line in dataFile:
    words = line.split(",")
    x.append(int(words[0]))
    y.append(int(words[1]))
    z.append(int(words[2]))

dataFile.close()

mat = np.array( z ).reshape((1800,3600))

plt.imshow(data[::-1,:], interpolation='none')
plt.colorbar()
plt.show()
plt.savefig("depthsPerRegion", dpi=300)





plt.imshow(data[::-1,:], interpolation='none')
plt.colorbar()
plt.show()
#plt.savefig("depthsPerRegion_1800x3600", dpi=300)



#-----------------------------------------------


