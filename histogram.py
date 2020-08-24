import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.axes_grid1 import make_axes_locatable
import sys

if len(sys.argv) > 1: 
	filename = sys.argv[1]

	try:
		data = np.genfromtxt(filename, dtype=np.uint, delimiter=',');
	except:
		print("[Error] Archivo no encontrado.")
	finally:
		s = 30
		print(data.shape)
		# data = data.reshape(2, s)

		fig, ax = plt.subplots()
		# ax.plot(data)

		ax.bar(data[:,0], data[:,1])
		plt.yscale('log')

		name = filename.split('.')[0]
		plt.savefig(name, bbox_inches='tight', dpi=300)

		plt.show()

else:
	print("[Error] Debe ingresar un nombre de archivo.")
