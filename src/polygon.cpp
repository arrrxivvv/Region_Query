// ***********************************************************************
//File            [ polygon.cpp ]
//Author          [ Algo group 8 ]
//Synopsis        [ Implementation, PolyMgr, Polygon, Edge, Point ]
//Date            [  ]
//Modified From   [ Lin, without problem 1~5 implementation ]
//Last Modified   [ 2013/06/14 14:05 Lin ]
//************************************************************************
#include"polygon.h"
#include<fstream>
#include<limits>
#include<algorithm>

using namespace std;
#define inf 10000

//double Edge::_scanx = 0;

/*struct intVertex
{
	int x;
	int y;
}*/

Edge::Edge(int x1, int y1, int x2, int y2, Edge* e, bool u, Polygon* parent)
		:_beginx(x1),_beginy(y1),_nextx(x2),_nexty(y2),_next(e),_up_or_down(u), _parentPoly(parent) {}

int mod( int a, int b )
{
	int r = a % b;
	if ( r < 0 ){
		r = r + b;
	}
	return r;
}

/*
int Edge::nextx() const{
	if(_next == 0)
		return inf;
	return (_next->_beginx);
}
int Edge::nexty()const {
	if(_next == 0)
		return inf;
	return (_next->_beginy);
}
*/
/*
double Edge::corry(double x)const{
//	cerr<<_scanx<<"---"<<endl;
	return ((x-_beginx)*(_nexty-_beginy)/(_nextx-_beginx) + _beginy);
}
int Edge::eq(int a){
	if(a == 0)
		return _nexty-_beginy;
	else if(a == 1)
		return _beginx-_nextx;
	else if(a == 2)
		return _beginx*_nexty-_beginy*_nextx;
	else
		return 0;
}
*/
/*
bool Edge::operator <(const Edge* & e) const{
	return corry()<e->corry();
}
*/

void Edge::setUpDown( bool updown )
{
	//swap begin and next if necessary
	if ( updown != _up_or_down ) {
		int tempxy;
		tempxy = _beginx;
		_beginx = _nextx;
		_nextx = tempxy;
		tempxy = _beginy;
		_beginy = _nexty;
		_nexty = tempxy;
	}
	_up_or_down = updown;
}

bool edgeCompLeft( const Edge* a, const Edge* b )
{
	if ( a->_beginx < b->_beginx ){
		return true;
	}
	else if ( a->_beginx > b->_beginx ) {
		return false;
	}
	else if ( a->_beginy < b->_beginy ) {
		return true;
	}
	else {
		return false;
	}
}

bool polyCompArea( const Polygon *A, const Polygon *B )
{
	return ( A->_area < B->_area );
}
/*
Point::Point(Edge* e1, Edge* e2, double x, int type)
		:_x(x),_type(type){

	if(type == 3){
		if(e1->_nexty > e2->_nexty)
			swap(e1,e2);
	}
	e[0]=e1;
	e[1]=e2;
}
*/
const string PolyMgr::command1( "FIND_INSIDE" );
const string PolyMgr::command2( "FIND_INTERACT" );
const string PolyMgr::command3( "FIND_NO_OVERLAP" );
const string PolyMgr::command4( "FIND_AREA_LESS_THAN" );
const string PolyMgr::command5( "FIND_DENSITY_INSIDE" );
PolyMgr::PolyMgr( string poly, string query, string out, bool verbose ):
	_polyFileName(poly), _queryFileName(query), _outFileName(out) 
{
	_isVerbose = verbose;
}

PolyMgr::PolyMgr()   //for testing only
{
	//for testing only
}

/*int PolyMgr::find_inside(int, int, int, int)
{
	//problem 1
	return 0;
}//end problem 1*/

/*int PolyMgr::find_interact(int, int, int, int)
{
	//problem 2
	return 0;
}//end problem 2*/

/*int PolyMgr::find_no_overlap(int, int, int, int)
{
	//problem 3
	return 0;
}//end problem 3*/

/*int PolyMgr::find_area_less_than(double)
{
	//problem 4
	return 0;
}//end problem 4
//problem 5*/
/*
double PolyMgr::find_density_inside(int x1,int x2,int y1,int y2){
	return 0;
} //end problem 5
*/
void PolyMgr::readPolygons()
{
	//clear all polygons

	//prepare data
	string keywordPoly("poly");
	ifstream polyFile( _polyFileName.c_str(), ios::in );
	string buffer;
	Polygon* tempPolyPtr = 0;
	vector<Vertex> Vs;  //temp store vertices
	Edge *thisEdge = 0;
	Edge *nextEdge = 0;
	Edge *prevEdge = 0;
	int leftmostone;
	int leftmostx;
	int rightmostone;
	int rightmostx;
	int highestone;
	int highesty;
	int lowestone;
	int lowesty;
	bool isdown;
	int iIncre;

	polyFile >> buffer;
	while ( polyFile.good() ){  //read polyFile
		if ( buffer == keywordPoly ) {
			tempPolyPtr = new Polygon;
			Vs.clear();
			polyFile >> tempPolyPtr->_numOfV;
			Vs.resize( tempPolyPtr->_numOfV );
			leftmostone = rightmostone = highestone = lowestone = 0;
			//cerr<<"lll"<<leftmostone<<endl;
			leftmostx = lowesty = numeric_limits<int>::max();
			rightmostx = highesty = numeric_limits<int>::min();
			//read in vertices, record boundary
			for ( int i = 0; i < Vs.size(); ++i){//read in vertices
				polyFile >> ( Vs[i] ).x;
				polyFile >> ( Vs[i] ).y;
				if ( Vs[i].x < leftmostx ) {
					leftmostx = Vs[i].x;
					leftmostone = i;
				}
				if ( Vs[i].x > rightmostx ) {
					rightmostx = Vs[i].x;
					rightmostone = i;
				}
				if ( Vs[i].y < lowesty) {
					lowesty = Vs[i].y;
					lowestone = i;
				}
				if ( Vs[i].y > highesty) {
					highesty = Vs[i].y;
					highestone = i;
				}

			}
			//put in vertices and boundary
			tempPolyPtr->_Vertices = Vs;
			tempPolyPtr->_highIndex = highestone;
			tempPolyPtr->_lowIndex = lowestone;
			tempPolyPtr->_rightIndex = rightmostone;
			tempPolyPtr->_leftIndex = leftmostone;
//				cout << Vs[highestone].y << " " << Vs[lowestone].y << " " 
//					 << Vs[rightmostone].x << " " << Vs[leftmostone].x << endl; //debuggg
			//compute area
			tempPolyPtr->computeArea();
			//construct edge list
			tempPolyPtr->_ups.clear();
			tempPolyPtr->_downs.clear();
			//determine direction, clockwise or not
			int firstone = mod(( leftmostone+1 ), Vs.size() );
			int lastone = mod( ( leftmostone-1 ), Vs.size() );
			double firstslope;
			double lastslope;
			if ( Vs[firstone].x == Vs[leftmostone].x ) { //first vertical
				if ( Vs[firstone].y > Vs[leftmostone].y ) {
					tempPolyPtr->_clockwise = true;
				}
				else {
					tempPolyPtr->_clockwise = false;
				}
			}
			else if ( Vs[lastone].x == Vs[leftmostone].x ){
				if ( Vs[lastone].y > Vs[leftmostone].y ) {
					tempPolyPtr->_clockwise = false;
				}
				else {
					tempPolyPtr->_clockwise = true;
				}
			}
			else {
				firstslope = double( Vs[firstone].y - Vs[leftmostone].y ) / double( Vs[firstone].x - Vs[leftmostone].x );
				lastslope = double( Vs[lastone].y - Vs[leftmostone].y ) / double( Vs[lastone].x - Vs[leftmostone].x );
				if ( firstslope > lastslope ) {
					tempPolyPtr->_clockwise = true;
				}
				else {
					tempPolyPtr->_clockwise = false;
				}
			}
			iIncre = ( tempPolyPtr->_clockwise ) ? 1 : -1;
			int current, next1, next2, prev1;  //index of Vs vector
			int firstvertical = 0;
			current = leftmostone;
			prev1 = mod( ( current - iIncre ), Vs.size() );
			next1 = mod( ( current + iIncre ), Vs.size() );
			next2 = mod( ( current + 2 * iIncre ), Vs.size() );
			//if first edge is vertical
			if ( Vs[next1].x == Vs[current].x ) {
				firstvertical = 1;
				leftmostone += iIncre; 
				current += iIncre;
				next1 += iIncre;
				next2 += iIncre;
				prev1 += iIncre;
				leftmostone = mod( leftmostone, Vs.size() );
				current = leftmostone;
				next1 = mod(next1, Vs.size());
				next2 = mod(next2, Vs.size());
				prev1 = mod(prev1, Vs.size());
			}
			else if ( Vs[prev1].x == Vs[current].x ){
				firstvertical = 1;
			}
			isdown = false; //first edge shoud be up
				//cout<<"---" << Vs[leftmostone].y << " " << Vs[next1].y << endl; debuggg
			nextEdge = new Edge( Vs[leftmostone].x, Vs[leftmostone].y, Vs[next1].x, Vs[next1].y, 0, false, tempPolyPtr );
			thisEdge = new Edge( Vs[leftmostone].x, Vs[leftmostone].y, Vs[prev1].x, Vs[prev1].y, 0, true, tempPolyPtr );
			Edge* firstup = nextEdge;  //record first up
				//cout << nextEdge->_beginy << " " << firstup->_beginy << endl; //debuggg
			//tempPolyPtr->_ups.push_back( nextEdge );
			//tempPolyPtr->_downs.push_back( thisEdge );
			tempPolyPtr->_ups.clear();
			tempPolyPtr->_downs.clear();
			for ( int i = 0; i * iIncre <= Vs.size()- 1 + firstvertical; i += iIncre ){
					//cout <<"construct edge list for" << endl;  //debuggg
				prevEdge = thisEdge;
				thisEdge = nextEdge;


				current = mod( ( i + leftmostone ), Vs.size() );
				next1 = mod( ( current + iIncre ), Vs.size() );
				next2 = mod( ( current + 2 * iIncre), Vs.size() );
				nextEdge = new Edge( Vs[next1].x, Vs[next1].y, Vs[next2].x, Vs[next2].y, 0, false, tempPolyPtr );

					//cout << thisEdge->_beginx << " " << thisEdge->_beginy << " " << thisEdge->_nextx << " " << thisEdge->_nexty << " " << nextEdge->_beginx << " " << nextEdge->_beginy << " " << nextEdge->_nextx << " " << nextEdge->_nexty << endl; //debuggg

				//vertical thisEdge?
				/*if ( thisEdge == 0 ) { //vertical thisEdge
					//replace it with previous one
						cout << "thisEdge = 0" << endl; //debuggg
					thisEdge = prevEdge;
					continue;
				}*/
				isdown = thisEdge->_up_or_down;
				//examine nextEdge
				if ( Vs[next2].x == Vs[next1].x ) {//vertical nextEdge
					delete nextEdge;
					//get the next edge
					next1 = mod( next1 + iIncre, Vs.size() );
					next2 = mod( next2 + iIncre, Vs.size() );
					i += iIncre;
				nextEdge = new Edge( Vs[next1].x, Vs[next1].y, Vs[next2].x, Vs[next2].y, 0, false, tempPolyPtr );
				}
				//proceed
				if ( ( Vs[next1].x > Vs[current].x ) == ( Vs[next2].x > Vs[next1].x ) ) {   //if direction is not changed
					nextEdge->setUpDown( isdown );
					thisEdge->_next = (isdown?  prevEdge : nextEdge );
				}
				else if ( isdown ) {   //meet a start pt
					thisEdge->_next = prevEdge;
					nextEdge->setUpDown(!isdown);
					tempPolyPtr->_downs.push_back( thisEdge );//record	
					tempPolyPtr->_ups.push_back( nextEdge );
						//cout << "pushback" << endl;
						//cout << thisEdge->_beginx << " " << thisEdge->_beginy << " " << thisEdge->_nextx << " " << thisEdge->_nexty << " " << nextEdge->_beginx << " " << nextEdge->_beginy << " " << nextEdge->_nextx << " " << nextEdge->_nexty << endl; //debuggg
					//cout << (tempPolyPtr->_ups[0])->_beginy << " " << (tempPolyPtr->_downs[0])->_beginy << endl; //debuggg

				}
				else {  //meet an end point
					nextEdge->setUpDown(!isdown);
					thisEdge->_next = 0;
					nextEdge->_next = 0;
					thisEdge = 0;  //mark the end point
				}
			} //end for
			sort( (tempPolyPtr->_ups).begin(), (tempPolyPtr->_ups).end(), edgeCompLeft );
			sort( (tempPolyPtr->_downs).begin(), (tempPolyPtr->_downs).end(), edgeCompLeft );
			(tempPolyPtr->_ups)[0] = firstup;  //replace first up
			delete nextEdge; //delete duplicated first up

				//remove duplicated first start point
			//tempPolyPtr->_ups.pop_back();
			//tempPolyPtr->_downs.pop_back();;

			//end construct edge list

			_polygons.push_back( tempPolyPtr );
			Edge *edgeout;
				//cout << "edges: " << endl; debuggg
			/*for ( int i = 0; i < tempPolyPtr->_ups.size(); ++i) { //debuggg
				edgeout = tempPolyPtr->_ups[i];
				cout << "up" << i << endl;
				while ( edgeout != 0 ){
					cout << edgeout->_beginx << " " << edgeout->_beginy << " "
						 << edgeout->_nextx << " " << edgeout->_nexty << endl;
					edgeout = edgeout->_next;
				}
			}
			for ( int i = 0; i < tempPolyPtr->_downs.size(); ++i) { //debuggg
				edgeout = tempPolyPtr->_downs[i];
				cout << "down" << i << endl;
				while ( edgeout != 0 ){
					cout << edgeout->_beginx << " " << edgeout->_beginy << " "
						 << edgeout->_nextx << " " << edgeout->_nexty << endl;
					edgeout = edgeout->_next;
				}
			}*/
		}
		polyFile >> buffer;
	}//end reading polyFile
	//sort( _polygons.begin(), _polygons.end(), polyCompArea );
	
	tempPolyPtr = 0;
	thisEdge = 0;
	nextEdge = 0;
	prevEdge = 0;
	//close file
	polyFile.close();
}//end readPolygons


void PolyMgr::respondQuery()
{
	ifstream queryFile( _queryFileName.c_str(), ios::in );
	ofstream outFile( _outFileName.c_str(), ios::out );
	string command;
	int xleft, ylow, xright, yhigh;
	double area;
	 ///debuggg print polygons
	/*for ( int i = 0; i < _polygons.size(); ++i ) {
		outFile << "poly" << " " << _polygons[i]->_numOfV << endl;
		for ( int j = 0; j < _polygons[i]->_numOfV; ++j ){
			if ( j != 0 && (j%4) == 0 ) {
				outFile << endl;
			}
			outFile << ( _polygons[i]->_Vertices )[j].x << " " << ( _polygons[i]->_Vertices )[j].y << " "; 
		}
		outFile << endl; 
	}*/
	queryFile >> command;
	while ( !queryFile.eof() ){
		//queryFile >> command;
		outFile << command << " ";
		if ( command == command4 ) { 
			queryFile >> area;
			outFile << area << endl;
			outFile << find_area_less_than( area ) << endl;
		}
		else if ( command == command1 ) { 
			queryFile >> xleft >> ylow >> xright >> yhigh;
			outFile << xleft << " " << ylow << " "<< xright << " " << yhigh << endl;
			outFile << find_inside( xleft, ylow, xright, yhigh ) << endl;

		}
		else if ( command == command2 ){  
			queryFile >> xleft >> ylow >> xright >> yhigh;
			outFile << xleft << " " << ylow << " "<< xright << " " << yhigh << endl;
			vector<Polygon*> outPolys;
			outPolys = find_interact( xleft, ylow, xright, yhigh );
			outFile << outPolys.size() << endl;
		}
		else if ( command == command3 ) { 
			queryFile >> xleft >> ylow >> xright >> yhigh;
			outFile << xleft << " " << ylow << " "<< xright << " " << yhigh << endl;

			outFile << find_no_overlap( xleft, ylow, xright, yhigh ) << endl;
		}
		else if ( command == command5 ) { 
			queryFile >> xleft >> ylow >> xright >> yhigh;
			outFile << xleft << " " << ylow << " "<< xright << " " << yhigh << endl;

			outFile << find_density_inside( xleft, ylow, xright, yhigh ) << endl;
		}
		queryFile >> command;
	}  //end while of queryFile
	queryFile.close();
	outFile.close();
}//end respondQuery
/*
double PolyMgr::findlength(set<Edge*,EdgeComp>* edges){
	int a = 0;
	double length =0;
	double start;
	for(set<Edge*,EdgeComp>::iterator it=edges->begin();it!=edges->end();++it){
		if(a==0)
			start = (*it)->corry();
		if(!((*it)->_up_or_down))
			a++;
		else
			a--;
		if(a==0)
			length+= start - (*it)->corry();
	}
	return length;
}

bool PolyMgr::CrossorNot(Edge* e1, Edge* e2, double& x){

	if(e1 == 0 || e2 == 0)
		return false;
	if(e1 -> _next == 0 || e2 -> _next == 0)
		return false;
	int delta[3] ={e1->eq(0)*e2->eq(1) - e2->eq(0)*e1->eq(1),
						e1->eq(2)*e2->eq(1) - e2->eq(2)*e1->eq(1),
						e1->eq(0)*e2->eq(2) - e2->eq(0)*e1->eq(2)};
	if(delta[0]!=0){
		if(x > delta[1]/delta[0])
			return false;
		x = delta[1]/delta[0];
		if(x >= e1->_nextx || x>= e2->_nextx )
			return false;
		
		return true;
	}else{
		if(delta[1]==0 && delta[2]==0){
			x = (e1->_nextx < e2->_nextx)?e1->_nextx:e2->_nextx;
			return true;
		}else
			return false;
			
	}
		
}
*/
void Polygon::computeArea()
{
	Vertex start = _Vertices[0];
	_area = 0;
	int x1, y1, x2, y2;
	for ( int i = 1; i <= _Vertices.size() - 2; ++i ){
		x1 = start.x - _Vertices[i].x;
		y1 = start.y - _Vertices[i].y;
		x2 = start.x - _Vertices[i+1].x;
		y2 = start.y - _Vertices[i+1].y;
		_area += x1 * y2 - x2 * y1; 
	}
	_area = _area / 2;
	if ( _area < 0 ){
		_area = (-1) * _area;
	}
}


