import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.axes_grid1 import make_axes_locatable
import sys

if len(sys.argv) > 1: 
	filename = sys.argv[1]

	try:
		data = np.genfromtxt(filename, delimiter=',');
	except:
		print("[Error] Archivo no encontrado.")
	finally:
		s = 2
		data = data[::-1,-1:].reshape(s*90, s*180)[:,::-1]
		# data = data[::-1,-1:].reshape(1024, 1024)[:,::-1]

		fig, ax = plt.subplots()
		# ax.plot(data)

		im = plt.imshow(data)
		divider = make_axes_locatable(ax)
		cax = divider.append_axes("right", size="5%", pad=0.05)
		plt.colorbar(im, cax=cax)

		name = filename.split('.')[0]
		plt.savefig(name, bbox_inches='tight', dpi=300)

		plt.show()

else:
	print("[Error] Debe ingresar un nombre de archivo.")
