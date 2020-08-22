
import numpy as np
import matplotlib.pyplot as plt
import os

#####################################################################
#####################################################################
##################         GRAPHS            ########################
#####################################################################
#####################################################################

#dataFile = open("citiesPerRegion_250x250.txt","r")
#dataFile = open("populationPerRegion_100x100.txt","r")
#dataFile = open("populationPerRegion_360x360.txt","r")
dataFile = open("depthsPerRegion_180x360.txt","r")

x = []
y = []
z = []

for line in dataFile:
    words = line.split(",")
    x.append(int(words[0]))
    y.append(int(words[1]))
    z.append(int(words[2]))
    #print(x,y,data)
#print(z)
dataFile.close()


mat = np.array( z ).reshape((180,360))

print(mat.shape)

plt.imshow(mat[::-1,:], interpolation='none')
plt.colorbar()
plt.show()



#-----------------------------------------------


