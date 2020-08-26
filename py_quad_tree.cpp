#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <python3.8/structmember.h>

#include "quad_tree.h"

using namespace std;

typedef struct {
	PyObject_HEAD;
	PyObject* tree;
} quad_tree;

static PyMemberDef quad_tree_members[] = {
	{ "tree", T_OBJECT, offsetof(quad_tree, tree), 0, "Quad Tree" },
	{ NULL }
};

static void quad_tree_dealloc(quad_tree* self) {
	Py_TYPE(self)->tp_free((PyObject *) self);
}

static PyObject* quad_tree_new(PyTypeObject *type, PyObject *args, PyObject *kwargs) {
	double lat;
	double lon;

	if (!PyArg_ParseTuple(args, "dd", &lat, &lon))
		return NULL;

	quad_tree* self;

	self = (quad_tree*)type->tp_alloc(type, 0);
	if (self != NULL) {
		self->tree = (PyObject*) new dsa::quad_tree(lat, lon);
	}

	return (PyObject*) self;
}

static int quad_tree_init(quad_tree* self, PyObject* args, PyObject* kwargs) {
	return 0;
}

static PyObject* quad_tree_dims(quad_tree* self) {
	PyObject* dims = PyTuple_New(2);

	auto [lat, lon] = ((dsa::quad_tree*)(self->tree))->dimensions();

	PyTuple_SET_ITEM(dims, 0, PyFloat_FromDouble(lat));
	PyTuple_SET_ITEM(dims, 1, PyFloat_FromDouble(lon));

	return dims;
}

static PyObject* quad_tree_insert(quad_tree* self, PyObject* args, PyObject* kwargs) {
	double lat;
	double lon;

	if (!PyArg_ParseTuple(args, "dd", &lat, &lon))
		return NULL;

	dsa::data_t d = {{lat, lon}, {}};

	long inserted = ((dsa::quad_tree*)(self->tree))->insert(d);

	return PyBool_FromLong(inserted);
}

static PyObject* quad_tree_remove(quad_tree* self, PyObject* args, PyObject* kwargs) {
	double lat;
	double lon;

	if (!PyArg_ParseTuple(args, "dd", &lat, &lon))
		return NULL;

	dsa::point_t p = {lat, lon};

	long removed = ((dsa::quad_tree*)(self->tree))->remove(p);

	return PyBool_FromLong(removed);
}

static PyObject* quad_tree_track(quad_tree* self, PyObject* args, PyObject* kwargs) {
	double lat;
	double lon;

	if (!PyArg_ParseTuple(args, "dd", &lat, &lon))
		return NULL;

	vector<dsa::quadrant::quad_enum> path;
	dsa::point_t p = {lat, lon};

	path = ((dsa::quad_tree*)(self->tree))->track(p);

	PyObject* tracks = PyList_New(path.size());

	for (size_t i = 0; i < path.size(); ++i) {
		string t = dsa::quadrant::quad_map[path[i]+1];
		PyList_SET_ITEM(tracks, i, PyUnicode_FromString(t.c_str()));
	}

	return tracks;
}

static PyObject* quad_tree_region_search(quad_tree* self, PyObject* args, PyObject* kwargs) {
	double lat;
	double lon;
	int d;

	if (!PyArg_ParseTuple(args, "ddi", &lat, &lon, &d))
		return NULL;

	dsa::quad_t q = {{lat, lon}, {d, d}};

	long points = ((dsa::quad_tree*)(self->tree))->get_total_cities(q);

	return PyLong_FromLong(points);
}

static PyMethodDef quad_tree_methods[] = {
	{ "dims",   (PyCFunction) quad_tree_dims,   METH_VARARGS, "dimensions of the tree" },
	{ "insert", (PyCFunction) quad_tree_insert, METH_VARARGS, "inserts a point"        },
	{ "remove", (PyCFunction) quad_tree_remove, METH_VARARGS, "removes a point"        },
	{ "track",  (PyCFunction) quad_tree_track,  METH_VARARGS, "tracks a point"         },
	{NULL}
};

static PyTypeObject quad_tree_type = {
	PyVarObject_HEAD_INIT(NULL, 0) 
	.tp_name = "quad_tree.quad_tree",						/* tp_name */
	.tp_basicsize = sizeof(quad_tree),						/* tp_basicsize */
	.tp_dealloc = (destructor) quad_tree_dealloc,			/* tp_dealloc */
	.tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,	/* tp_flags */
	.tp_doc = "quad_tree object",							/* tp_doc */
	.tp_methods = quad_tree_methods,						/* tp_methods */
	.tp_members = quad_tree_members,						/* tp_members */
	.tp_init = (initproc)quad_tree_init,					/* tp_init */
	.tp_new = quad_tree_new,								/* tp_new */
};

static struct PyModuleDef quad_tree_module = {
	PyModuleDef_HEAD_INIT,
	"Quad Tree",
	"Quad Tree data structure",
	-1,
	quad_tree_methods
};

PyMODINIT_FUNC PyInit_quad_tree(void) {
	Py_Initialize();
	PyObject *m = PyModule_Create(&quad_tree_module);

	if (PyType_Ready(&quad_tree_type) < 0)
	return NULL;

	Py_INCREF(&quad_tree_type);
	if (PyModule_AddObject(m, "quad_tree", (PyObject *)&quad_tree_type) < 0) {
		Py_DECREF(&quad_tree_type);
		Py_DECREF(m);
		return NULL;
	}

	return m;
}
