// ***********************************************************************
//File            [ polygon.h ]
//Author          [ Algo group 8 ]
//Synopsis        [ Header File, PolyMgr, Polygon, Edge, Point ]
//Date            [  ]
//Modified From   [ Lin, without problem 1~5 implementation ]
//Last Modified   [ 2013/06/04 11:50 Lin ]
//************************************************************************
#ifndef POLYGON_H
#define POLYGON_H

#include<iostream>
#include<vector>
#include<set>
#include<string>
using namespace std;

class Polygon;
class Edge;
class Point;
struct EdgeComp;
struct PointComp;
struct Vertex;

class PolyMgr
{
public:
	PolyMgr( string, string, string, bool );   //initialize filename and mode
	PolyMgr();		//for testing only

	//the five problems
	int find_inside(int, int, int, int);
	vector<Polygon*> find_interact(int, int, int, int);
	int find_no_overlap(int, int, int, int);
	int find_area_less_than(double);
	
	double find_density_inside(int,int,int,int);
	

	//system control
	void readPolygons( );
	void respondQuery( );
	

	//utilities
	//Q5
	double findlength(set<Edge*,EdgeComp>*);
	void check_new_cross(set<Point*,PointComp>& points,
	set<Edge*,EdgeComp>& edges, Edge* e, double x_now);
	bool CrossorNot(Edge* e1, Edge* e2, double& x);

	//data
	vector<Polygon*> _polygons;
	string _polyFileName;
	string _queryFileName;
	string _outFileName;
	bool _isVerbose;

	//static data
	static const string command1;
	static const string command2;
	static const string command3;
	static const string command4;
	static const string command5;

};

class Polygon
{
public:
	void computeArea();

	int _numOfV;    //the number of vertices
	//ups and downs for problem 5
	vector<Edge*> _ups;
	vector<Edge*> _downs;

	vector<Vertex> _Vertices;
	// top, bottom etc for problem 1, 2
	Edge* _up;
	Edge* _down;
	Edge* _highest;
	Edge* _lowest;
	// boundary point index in _Vertices
	int _highIndex;
	int _lowIndex;
	int _leftIndex;
	int _rightIndex;

	double _area;
	bool _clockwise;
	bool _flag;
};

class Edge
{
public:
	Edge(int,int,int,int,Edge*,bool, Polygon*);

	static double _scanx;
	double slope() const;
	double corry(double x = _scanx) const;
//	int nextx() const;
//	int nexty() const;
	int eq(int a);
	void setUpDown( bool );
//	bool operator < (const Edge* &) const;
		
	int _beginx;
	int _beginy;
	int _nextx;
	int _nexty;

	Edge* _next;
	Edge* _previous;

	Polygon* _parentPoly;

	bool _up_or_down;   //up 0, down 1 use setUpDown to set
	int _num;
};

struct EdgeComp
{
	static bool _pretime;
	bool operator() (const Edge* e1, const Edge* e2) const
	{
		//cerr<<"comparing"<<endl;
//		return 0;
		if(e1->corry() == e2->corry()){
//			cout<<e1->_scanx <<" "<<e2->_scanx<<" "<<e1->_beginy<<" "<<e2->_beginy<<endl;
			if( e1->slope() == e2->slope()){
//				cerr<< (size_t)e1 <<" "<< (size_t)e2 <<endl;
				return (size_t)e1 < (size_t)e2;
			}	
			if(_pretime)
				return e1->slope() < e2->slope();
			else
				return e1->slope() > e2->slope();
		}else{
//			cout<<e1->_scanx <<" "<<e2->_scanx<<endl;
			return e1->corry() > e2->corry();
			
		}
	}
};
class Point
{
public:
	Point(Edge* , Edge*,double,double,int);
	
	int _type;
	double _x;
	double _y;
	Edge* e[2];
};
struct PointComp
{
	bool operator() (const Point* p1, const Point* p2) const
	{
		cout<<"pointcomp"<<endl;
		if(p1 -> _x == p2 -> _x){
			if(p1->e[0]->corry() != p2->e[0]->corry() )
				return p1->e[0]->corry(p1->_x) > p2->e[0]->corry(p2->_x);
			if(p1->_type != p2->_type)
				return p1->_type > p2->_type;
			if( (size_t)p1->e[0] == (size_t)p2 ->e[0])
				return (size_t)p1->e[1] < (size_t)p2 ->e[1];
			else
				return (size_t)p1->e[0] < (size_t)p2 ->e[0];
		}
//			return p1->e[0]->corry(p1->_x) < p2->e[0]->corry(p2->_x);
		return p1->_x < p2->_x;
	}
};

struct Vertex
{
	int x;
	int y;
};

struct Cross
{
	double y;
	bool is_vertex;
	bool is_vertical;
	bool sharp_vertex;
};

#endif
