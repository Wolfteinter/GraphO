#include <Python.h>
#include<iostream>
using namespace std;
double eps = 1e-9;
bool geq(double a, double b){return a-b >= -eps;}     //a >= b
bool leq(double a, double b){return b-a >= -eps;}     //a <= b
bool ge(double a, double b){return a-b > eps;}        //a > b
bool le(double a, double b){return b-a > eps;}        //a < b
bool eq(double a, double b){return abs(a-b) <= eps;}  //a == b
bool neq(double a, double b){return abs(a-b) > eps;}  //a != b

struct point{
    double x,y;
    point(): x(0), y(0){}
	point(double x, double y): x(x), y(y){}
    point operator+(const point & p) const{return point(x + p.x, y + p.y);}
	point operator-(const point & p) const{return point(x - p.x, y - p.y);}
	point operator*(const double & k) const{return point(x * k, y * k);}
	point operator/(const double & k) const{return point(x / k, y / k);}
	point operator+=(const point & p){*this = *this + p; return *this;}
	point operator-=(const point & p){*this = *this - p; return *this;}
	point operator*=(const double & p){*this = *this * p; return *this;}
	point operator/=(const double & p){*this = *this / p; return *this;}
    double dot(const point & p) const{
        return x * p.x + y * p.y;
    }
    double cross(const point & p) const{
        return x * p.y - y * p.x;
    }
};

/*
Input: number of points and a list of points
Output:
*/
static PyObject* graphO_to_disjointness_graph(PyObject* self, PyObject* args){
    PyObject *pList;
    PyObject *pItem;
    double x,y;//x and y values
    if (!PyArg_ParseTuple(args, "O!", &PyList_Type, &pList)) {
        return NULL;
    }
    PyObject* python_val = PyList_New(PyList_Size(pList));
    if (PyList_Check(pList)) {
        for(Py_ssize_t i = 0; i < PyList_Size(pList); i++) {
			PyObject *value = PyList_GetItem(pList, i);
            PyArg_Parse(value, "O!", &PyTuple_Type, &pItem);
            if (PyTuple_Check(pItem)) {
                x = PyFloat_AsDouble(PyTuple_GetItem(pItem,0));
                y = PyFloat_AsDouble(PyTuple_GetItem(pItem,1));
                PyObject* python_int = Py_BuildValue("d",x + y);
                PyList_SetItem(python_val, i, python_int);
            }
			//data.push_back( PyFloat_AsDouble(value) );
		}
        return python_val;
    }
}
static PyMethodDef graphO_Methods[] = {
     {"to_disjointness_graph", graphO_to_disjointness_graph, METH_VARARGS, "Working"},
     {NULL, NULL, 0, NULL}
};

static struct PyModuleDef moduledef = {
        PyModuleDef_HEAD_INIT,
        "graphO",
        NULL,
        -1,
        graphO_Methods
};

PyMODINIT_FUNC PyInit_graphO(void){
     PyObject *module = PyModule_Create(&moduledef);
     return module;
}
