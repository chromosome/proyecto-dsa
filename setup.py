from distutils.core import setup, Extension

quad_tree_module = Extension('quad_tree',
							 # include_dirs=['.'],
							 sources=['py_quad_tree.cpp'],
							 extra_compile_args=['-std=c++17'],
							 language='c++'
							)

setup(name='quad_tree', 
	  version='0.1.0', 
	  description='Quad Tree Data Structure',
	  author='Danilo Bonometti',
	  author_email='danilobonometti@udec.cl',
	  ext_modules=[quad_tree_module])
