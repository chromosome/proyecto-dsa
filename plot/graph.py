
import numpy as np
import matplotlib.pyplot as plt
import os

#####################################################################
#####################################################################
##################         GRAPHS            ########################
#####################################################################
#####################################################################

dataFile = open("histogram2D.txt","r")

x = []
y = []
z = []

for line in dataFile:
    words = line.split(",")
    x.append(int(words[0]))
    y.append(int(words[1]))
    z.append(int(words[2]))
    #print(x,y,data)
print(z)
dataFile.close()


mat = np.array( z, dtype=np.uint8).reshape((25,25)).transpose()

print(mat.shape)

plt.imshow(mat, interpolation='none')

plt.show()


#plt.imshow(z)
#plt.xticks(range(len(x)), x)
#plt.yticks(range(len(y)), y)
#plt.show()

#-----------------------------------------------


