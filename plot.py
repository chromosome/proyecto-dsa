import matplotlib.pyplot as plt
import numpy as np
from scipy.interpolate import make_interp_spline, BSpline

def read_data(filename):
	data = np.genfromtxt(filename)
	return data

def plot(name, title, programs, x_label='n', y_label='Tiempo [ms]', 
		 samples=None, interp=2, x_scale='log', y_scale='linear', 
		 x_lim=None, y_lim=None):
	path = 'results/'

	fig, ax = plt.subplots()
	# fig.suptitle(title)
	for program in programs:
		data = read_data(path+program+".dat")

		# x = np.linspace(n.min(), n.max(), 1000)
		# spline = make_interp_spline(n, t, k=interp)
		# y = spline(x)
		ax.plot(data, label=programs[program])

	ax.set_xlabel(x_label)
	ax.set_ylabel(y_label)
	if x_lim:
		ax.set_xlim(x_lim)
	if y_lim:
		ax.set_ylim(y_lim)
	ax.set_xscale(x_scale)
	ax.set_yscale(y_scale)

	plt.legend(loc="upper left")
	plt.savefig(name, bbox_inches='tight', transparent=True, dpi=200)
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
	plot('insertion_time', 
		 'insertion_time', 
		 {'insertion_time': 'PR-Quad Tree'},
		 interp=1,
		 x_scale='linear')
