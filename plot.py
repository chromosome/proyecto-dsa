import matplotlib.pyplot as plt
import numpy as np
from scipy.interpolate import make_interp_spline, BSpline

def read_data(filename):
	data = np.genfromtxt(filename, delimiter=',')
	return data

def plot(name, title, programs, references=dict(), x_label='n', y_label='Tiempo [ms]', 
		 samples=None, interp=2, x_scale='log', y_scale='linear', 
		 x_lim=None, y_lim=None):
	path = 'results/'

	fig, ax = plt.subplots()
	# fig.suptitle(title)
	for program in programs:
		print(program)
		data = read_data(path+program+".dat")

		# x = np.linspace(n.min(), n.max(), 1000)
		# spline = make_interp_spline(n, t, k=interp)
		# y = spline(x)
		ax.plot(data, label=programs[program])

	for l, f in references.items():
		x = len(data)
		ax.plot(f(np.arange(1, x)), label=l)

	ax.set_xlabel(x_label)
	ax.set_ylabel(y_label)
	if x_lim:
		ax.set_xlim(x_lim)
	if y_lim:
		ax.set_ylim(y_lim)
	ax.set_xscale(x_scale)
	ax.set_yscale(y_scale)

	plt.legend(loc="best")
	plt.savefig(name, bbox_inches='tight', transparent=True, dpi=300)
	plt.show()


def bar_plot(name, title, programs, x_label='n', y_label='Tiempo [ms]'):
	path = 'results/'

	fig, ax = plt.subplots()
	# fig.suptitle(title)
	width = 1000
	i = 0
	for program in programs:
		n, t = read_data(path+program)
		ax.bar(n+i, t, width, label=programs[program])
		i += 500
	ax.set_xlabel(x_label)
	ax.set_ylabel(y_label)

	plt.legend(loc="upper left")
	plt.savefig(name, bbox_inches='tight', transparent=True, dpi=300)


def hist(name, title, programs, x_label='n', y_label='Tiempo [ms]'):
	path = 'results/'

	fig, ax = plt.subplots()
	# fig.suptitle(title)
	width = 1000
	i = 0
	for program in programs:
		n, t = read_data(path+program)
		ax.bar(n+i, t, width, label=programs[program])
		i += 500
	ax.set_xlabel(x_label)
	ax.set_ylabel(y_label)

	plt.legend(loc="upper left")
	plt.savefig(name, bbox_inches='tight', transparent=True, dpi=300)


if __name__ == '__main__':
	# plot('insertion_time', 
	# 	 'insertion_time', 
	# 	 {'insertion_time': 'inserción'},
	# 	 # references= { 'log4(x)' : (lambda x: 0.5*np.log2((3*x+1)/4)) },
	# 	 interp=1,
	# 	 x_scale='linear')

	# plot('insertion_depth', 
	# 	 'Profundidad Promedio', 
	# 	 {'insertion_depth': 'Profundidad ponderada'},
	# 	 y_label='Profundidad',
	# 	 interp=1,
	# 	 x_scale='linear')

	# plot('deletion_time', 
	# 	 'Eliminación', 
	# 	 {'deletion_time': 'Eliminación'},
	# 	 interp=1,
	# 	 x_scale='linear')

	# plot('ocuppancy', 
	# 	 'ocuppancy', 
	# 	 {'ocuppancy': 'PR-Quad Tree'},
	# 	 interp=1,
	# 	 x_scale='linear')

	# plot('query_time', 
	# 	 'query_time', 
	# 	 {'query_time': 'PR-Quad Tree'},
	# 	 interp=1,
	# 	 x_scale='linear')

	plot('region_query_time', 
		 'region_query_time', 
		 {'region_query_time': 'Consultas por Región al Azar'},
		 interp=1,
		 x_scale='linear')

	# plot('consultas por punto', 
	# 	 'Consultas por Punto', 
	# 	 {
	# 	 	'insertion_time': 'Inserción',
	# 	 	'insertion_time_random': 'Inserción al Azar',
	# 	 	'deletion_time': 'Eliminación',
	# 	 	'deletion_time_random': 'Eliminación al Azar',
	# 	 	# 'query_time': 'Búsqueda',
	# 	 	'query_time_random': 'Búsqueda al Azar'
	# 	 },
	# 	 interp=1,
	# 	 x_scale='linear')
	
