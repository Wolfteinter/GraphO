#include <Python.h>
#include <vector>
#include <iostream>
#include <map>
using namespace std;
double eps = 1e-9;
bool geq(double a, double b){return a - b >= -eps;}     //a >= b
bool leq(double a, double b){return b - a >= -eps;}     //a <= b
bool ge(double a, double b){return a - b > eps;}        //a > b
bool le(double a, double b){return b - a > eps;}        //a < b
bool eq(double a, double b){return abs(a - b) <= eps;}  //a == b
bool neq(double a, double b){return abs(a - b) > eps;}  //a != b

struct point{
    double x,y;
    int pos;
    point(): x(0), y(0){}
    point(double x, double y): x(x), y(y){}
	point(double x, double y , int pos): x(x), y(y), pos(pos){}
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
struct segment{
    point a,b;
    int id;
    segment(point a, point b): a(a), b(b){}
    segment(point a, point b , int id): a(a), b(b),id(id){}
};

vector<segment> generate_segments(vector<point> points){
    int sz = (int)points.size();
    vector<segment> segments;
    //Create all the segments with all the combinations of the points O(n(n-1)/2)
    for(int i = 0; i < sz ;i++){
        for(int j = i + 1; j < sz ;j++){
            segments.push_back(segment(points[i] , points[j]));
        }
    }
    return segments;
}
//function sign
int sgn(double x){
	if(ge(x, 0)) return 1;
	if(le(x, 0)) return -1;
	return 0;
}
//Know if a point is in a line
bool pointInLine(const point & a, const point & v, const point & p){
	//line a+tv, point p
	return eq((p - a).cross(v), 0);
}
//Know if a point is in a segment
bool pointInSegment(const point & a, const point & b, const point & p){
	//Segment ab, point p
	return pointInLine(a, b - a, p) && leq((a - p).dot(b - p), 0);
}
//Know if two segments are disjoints
bool are_disjoints(const segment &s1 , const segment &s2){
    point a = s1.a;
    point b = s1.b;
    point c = s2.a;
    point d = s2.b;
    //Segment ab, segment cd
    point v1 = b - a, v2 = d - c;
    int t = sgn(v1.cross(c - a)), u = sgn(v1.cross(d - a));
	if(t == u){
		if(t == 0){
			if(pointInSegment(a, b, c) || pointInSegment(a, b, d) || pointInSegment(c, d, a) || pointInSegment(c, d, b)){
                return false;
			}else{
                return true;
			}
		}else{
            return true;
		}
	}else{
        return (sgn(v2.cross(a - c)) != sgn(v2.cross(b - c))) == 0 ? true : false;
	}
}
class AlgorithmA{
    public:
        vector<segment> AA;
        vector<segment> AB;
        vector<segment> AC;
        vector<segment> AD;
        vector<segment> BB;
        vector<segment> BC;
        vector<segment> BD;
        vector<segment> CC;
        vector<segment> CD;
        vector<segment> DD;
        vector<vector<int>> matrix;
    AlgorithmA(){
        AA.clear();
        AB.clear();
        AC.clear();
        AD.clear();
        BB.clear();
        BC.clear();
        BD.clear();
        CC.clear();
        CD.clear();
        DD.clear();
    }
    point calculate_center(vector<point> points){
        double x_sum = 0;
        double y_sum = 0;
        int sz = (int)points.size();
        for(int i = 0;i < sz;++i){
            x_sum += points[i].x / sz;
            y_sum += points[i].y / sz;
        }
        return point(x_sum , y_sum);
    }
    int get_plane(point a , point center){
        if(a.x <= center.x && a.y >= center.y)return 0;
        if(a.x > center.x && a.y >= center.y)return 1;
        if(a.x <= center.x && a.y < center.y)return 2;
        if(a.x > center.x && a.y < center.y)return 3;
    }
    vector<point> sort_points(vector<point> points,point center){
        vector<point> A;
        vector<point> B;
        vector<point> C;
        vector<point> D;
        int sz = (int)points.size();
        for(int i = 0;i < sz;++i){
            int plane = get_plane(points[i],center);
            if(plane == 0)A.push_back(points[i]);
            else if(plane == 1)B.push_back(points[i]);
            else if(plane == 2)C.push_back(points[i]);
            else if(plane == 3)D.push_back(points[i]);
        }
        vector<point> result;
        result.reserve(A.size() + B.size() + C.size() + D.size());
        result.insert(result.end(), A.begin(), A.end());        
        result.insert(result.end(), B.begin(), B.end());
        result.insert(result.end(), C.begin(), C.end());
        result.insert(result.end(), D.begin(), D.end());        
        return result;

    }
    void divide_in_quadrant(vector<point> points, point center){
        int sz = (int) points.size();
        int id = 0;
        //Generate the segments and clasificate them in his set that belongs
        for(int i = 0;i < sz ;++i){
            for(int j = i + 1;j < sz ;++j){
                point a = points[i];
                point b = points[j];
                int a_plane = get_plane(a,center);
                int b_plane = get_plane(b,center);
                //AA
                if(a_plane == 0 && b_plane == 0)AA.push_back(segment(a , b , id));
                //AB
                else if(a_plane == 0 && b_plane == 1)AB.push_back(segment(a , b , id));
                //AC
                else if(a_plane == 0 && b_plane == 2)AC.push_back(segment(a , b , id));
                //AD
                else if(a_plane == 0 && b_plane == 3)AD.push_back(segment(a , b , id));
                //BB
                else if(a_plane == 1 && b_plane == 1)BB.push_back(segment(a , b , id));
                //BC
                else if(a_plane == 1 && b_plane == 2)BC.push_back(segment(a , b , id));
                //BD
                else if(a_plane == 1 && b_plane == 3)BD.push_back(segment(a , b , id));
                //CC
                else if(a_plane == 2 && b_plane == 2)CC.push_back(segment(a , b , id));
                //CD
                else if(a_plane == 2 && b_plane == 3)CD.push_back(segment(a , b , id));
                //DD
                else if(a_plane == 3 && b_plane == 3)DD.push_back(segment(a , b , id));

                id++;
            }
        }

    }
    void check(vector<segment> X,vector<segment> Y){
        if((int)X.size() == 0 || (int)Y.size() == 0)return;
        //cout<<X.size()<<" "<<Y.size()<<endl;
        for(int i = 0;i < (int)X.size();++i){
            for(int j = 0;j < (int)Y.size();++j){
                //cout<<X[i].id<<" "<<Y[j].id<<endl;
                if(!are_disjoints(X[i], Y[j])){
                    this->matrix[X[i].id][Y[j].id] = 0;
                    this->matrix[Y[j].id][X[i].id] = 0;
                }
            }
        }
    }
    vector<vector<int>> solve(vector<point> points){
        int sz = (int)points.size();
        int sz_p = sz * (sz - 1) / 2;
        point center = calculate_center(points);
        vector<point> new_points = sort_points(points,center);
        divide_in_quadrant(new_points,center);
        //cout<<"tam"<<AA.size()<<endl;
        //Inicialize the matrix with all in 1 that means that all the nodes are adjacent
        //int matrix[sz_p][sz_p] = {1};
        //matrix = new vector<vector<int>>(sz_p,vector<int>(sz_p,1));
        matrix.resize(sz_p,vector<int>(sz_p,1));
        //AA
        check(AA,AA);
        check(AA,AB);
        check(AA,AC);
        check(AA,AD);
        check(AA,BC);
        //AB
        check(AB,AB);
        check(AB,AC);
        check(AB,AD);
        check(AB,BB);
        check(AB,BC);
        check(AB,BD);
        //AC
        check(AC,AC);
        check(AC,AD);
        check(AC,BC);
        check(AC,CC);
        check(AC,CD);
        //AD
        check(AD,AD);
        check(AD,BB);
        check(AD,BC);
        check(AD,BD);
        check(AD,CC);
        check(AD,CD);
        check(AD,DD);
        //BB
        check(BB,BB);
        check(BB,BC);
        check(BB,BD);
        //BC
        check(BC,BC);
        check(BC,BD);
        check(BC,CC);
        check(BC,CD);
        check(BC,DD);
        //BD
        check(BD,BD);
        check(BD,CD);
        check(BD,DD);
        //CC
        check(CC,CC);
        check(CC,CD);
        //CD
        check(CD,CD);
        check(CD,DD);
        //DD
        check(DD,DD);

        AA.clear();
        AB.clear();
        AC.clear();
        AD.clear();
        BB.clear();
        BC.clear();
        BD.clear();
        CC.clear();
        CD.clear();
        DD.clear();

        return matrix;
    }
};
/*
    Name   :  to_segments
    Input  :  A list of points P in the form [(x,y),(x,y)]
    Output :  A list of tuples with the positions of the points, with the form [(a,b),(a,b)] where a and b are the position in the list of the points, and every tuple is a segment, a to b
*/
static PyObject* graphO_to_segments(PyObject* self, PyObject* args){
    PyObject *pList;//The list of tuples
    PyObject *pItem;//A tuple of the list
    double x,y;//x and y values of the tuple
    vector<point> P;//The set of points
    vector<segment> P_;//The set of segments generated by P
    //Casting the Object to a PythonObject with form of list
    if (!PyArg_ParseTuple(args, "O!", &PyList_Type, &pList))return NULL;
    //Check if the object is a list
    if (PyList_Check(pList)) {
        //Iterate all the elements of the list
        for(Py_ssize_t i = 0; i < PyList_Size(pList); i++) {
            //Get any element
			PyObject *value = PyList_GetItem(pList, i);
            //Parse it to a tuple
            if(!PyArg_Parse(value, "O!", &PyTuple_Type, &pItem))return NULL;
            //Check if is a tuple
            if (PyTuple_Check(pItem)) {
                //Get the first value of the tuple that corresponds to the x value
                x = PyFloat_AsDouble(PyTuple_GetItem(pItem,0));
                //Get the second value of the tuple that corresponds to the y value
                y = PyFloat_AsDouble(PyTuple_GetItem(pItem,1));
                //Push back in the vector a new point using the x y values
                P.push_back(point(x, y ,(int)i));
            }
		}
        AlgorithmA alg;
        point center = alg.calculate_center(P);
        vector<point> new_points = alg.sort_points(P,center);
        //Generate all the segments
        P_ = generate_segments(new_points);
        //Create a objectlist
        PyObject* py_segments = PyList_New((int)P_.size());
        //Iterate all the segments
        for(int i = 0;i < (int) P_.size();i++){
            segment x_segment = P_[i];
            //Create a tuple with size of 2
            PyObject* py_tuple = PyTuple_New(2);
            //Set the first value of the tuple with the value of the position of the point a
            PyTuple_SetItem(py_tuple, 0 , Py_BuildValue("i",x_segment.a.pos));
            //Set the second value of the tuple with the value of the position of the point b
            PyTuple_SetItem(py_tuple, 1 , Py_BuildValue("i",x_segment.b.pos));
            //Set every element in the list with the value of the tuple
            PyList_SetItem(py_segments, i, py_tuple);
        }
        //Return the segments
        return py_segments;
    }
}
/*
    Name   :  to_disjointness_graph
    Input  :  A list of points P in the form [(x,y),(x,y)]
    Output :  A adyacency list of the graph with the form {0:[a,b,c]....}
*/
static PyObject* graphO_to_disjointness_graph_A(PyObject* self, PyObject* args){
    PyObject *pList;//The list of tuples
    PyObject *pItem;//A tuple of the list
    double x,y;//x and y values of the tuple
    vector<point> P;//The set of points max size 536870912

    //Casting the Object to a PythonObject with form of list
    if (!PyArg_ParseTuple(args, "O!", &PyList_Type, &pList))return NULL;

    //Check if the object is a list
    if (PyList_Check(pList)) {
        //Iterate all the elements of the list
        for(Py_ssize_t i = 0; i < PyList_Size(pList); i++) {
            //Get any element
			PyObject *value = PyList_GetItem(pList, i);
            //Parse it to a tuple
            if(!PyArg_Parse(value, "O!", &PyTuple_Type, &pItem))return NULL;
            //Check if is a tuple
            if (PyTuple_Check(pItem)) {
                //Get the first value of the tuple that corresponds to the x value
                x = PyFloat_AsDouble(PyTuple_GetItem(pItem,0));
                //Get the second value of the tuple that corresponds to the y value
                y = PyFloat_AsDouble(PyTuple_GetItem(pItem,1));
                //Push back in the vector a new point using the x y values
                P.push_back(point(x,y));
            }
		}
        AlgorithmA alg;
        vector<vector<int>> matrix = alg.solve(P);
        PyObject *adj_list = PyDict_New();
        for(int i = 0;i < (int)matrix.size() ;i++){//
            PyObject* neighbors_list = PyList_New(0);//Max size 536870912
            for(int j = i+1;j < (int)matrix.size() ;j++){
                if(i != j)
                    if(matrix[j][i] == 1)PyList_Append(neighbors_list,Py_BuildValue("i" , j));     
            }
            PyDict_SetItem(adj_list,Py_BuildValue("i" , i), neighbors_list);

        }
        matrix.clear();
        return adj_list;
        

    }
}
static PyObject* graphO_to_disjointness_graph_BF(PyObject* self, PyObject* args){
    PyObject *pList;//The list of tuples
    PyObject *pItem;//A tuple of the list
    double x,y;//x and y values of the tuple
    vector<point> P;//The set of points max size 536870912
    vector<segment> P_;//The set of segments generated by P

    //Casting the Object to a PythonObject with form of list
    if (!PyArg_ParseTuple(args, "O!", &PyList_Type, &pList))return NULL;

    //Check if the object is a list
    if (PyList_Check(pList)) {
        //Iterate all the elements of the list
        for(Py_ssize_t i = 0; i < PyList_Size(pList); i++) {
            //Get any element
			PyObject *value = PyList_GetItem(pList, i);
            //Parse it to a tuple
            if(!PyArg_Parse(value, "O!", &PyTuple_Type, &pItem))return NULL;
            //Check if is a tuple
            if (PyTuple_Check(pItem)) {
                //Get the first value of the tuple that corresponds to the x value
                x = PyFloat_AsDouble(PyTuple_GetItem(pItem,0));
                //Get the second value of the tuple that corresponds to the y value
                y = PyFloat_AsDouble(PyTuple_GetItem(pItem,1));
                //Push back in the vector a new point using the x y values
                P.push_back(point(x,y));
            }
		}
        AlgorithmA alg;
        point center = alg.calculate_center(P);
        vector<point> new_points = alg.sort_points(P,center);
        P_ = generate_segments(new_points);
        int P_sz = (int)P_.size();
        PyObject *adj_list = PyDict_New();
        map<pair<int,int> , int> duplicates;
        //Create the adjacency list in O(n*n) time complexity

        for(int i = 0;i < P_sz ;i++){//
            PyObject* neighbors_list = PyList_New(0);//Max size 536870912
            for(int j = 0;j < P_sz ;j++){
                //&& (P_[i].a != P_[j].a) && (P_[i].a != P_[j].b) && (P_[i].b != P_[j].a) && (P_[i].b != P_[j].b)
                if (i != j)
                    if(duplicates[make_pair(j,i)] == 0){
                        if(are_disjoints(P_[i] , P_[j]))PyList_Append(neighbors_list,Py_BuildValue("i" , j));
                        duplicates[make_pair(i,j)] = 1;
                    }
            }
            PyDict_SetItem(adj_list,Py_BuildValue("i" , i), neighbors_list);

        }
        return adj_list;
    }
}
static PyMethodDef graphO_Methods[] = {
     {"to_disjointness_graph_BF", graphO_to_disjointness_graph_BF, METH_VARARGS, "Working"},
     {"to_disjointness_graph_A", graphO_to_disjointness_graph_A, METH_VARARGS, "Working"},
     {"to_segments", graphO_to_segments, METH_VARARGS, "Working"},
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
