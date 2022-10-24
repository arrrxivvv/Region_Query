#include<iostream>
#include<string>
#include"polygon.h"
using namespace std;

int main( int argc, char* argv[] )
{
	// argv arguments
	string argPoly;
	string argQuery;
	string argOut;
	string tempVerbose;
	string verbose("-verbose");
	bool argVerbose = false;
	int argStart = 1;
	if ( argc == 4 ){
		argStart = 1;
		argVerbose = false;
	}
	else if ( argc == 5 ){
		tempVerbose = argv[2];
		if ( tempVerbose == verbose ){
			argStart = 2;
			argVerbose = true;
		}
		else {
			cout << "wrong input";
		}
	}
	else {
		cout << "wrong input";
	}
	argPoly= argv[ argStart ];
	argQuery= argv[ argStart+1 ];
	argOut= argv[ argStart+2 ];
	//instantiate PolyMgr
	PolyMgr manager( argPoly, argQuery, argOut, argVerbose );
	manager.readPolygons();
	manager.respondQuery();

/*//	Edge* e11 = new Edge(2,1,3,0,0,0);
	//Edge* e01 = new Edge(0,1,3,1,0,0);
//	Edge* e10 = new Edge(1,0,0,1);
	//Edge* e00 = new Edge(0,0,3,0,0,1);
	Polygon* square = new Polygon();
	square->_up = e01;
	square->_down = e00;
	manager._polygons.push_back(square);

	Edge* e12 = new Edge(1,2,2,2,0,0);
	Edge* e1m1 = new Edge(1,-1,2,-1,0,1);
	Polygon* rect = new Polygon();
	rect->_up = e12;
	rect->_down = e1m1;
	manager._polygons.push_back(rect);

	cout<<manager.find_density_inside(0,0,0,0)<<endl;*/
}
