// ***********************************************************************
//File            [ polygon5.cpp ]
//Author          [ Algo group 8 ]
//Synopsis        [ Problem 5 Implementation: find_density_inside ]
//Date            [  ]
//Modified From   [ Tuna ]
//Last Modified   [ 2013/06/08 Tuna ]
//************************************************************************
#include"polygon.h"
using namespace std;
#define inf 10000

double Edge::_scanx = 0;
bool EdgeComp::_pretime = 0;


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
double Edge::slope()const{
	return (double)(_nexty-_beginy)/(double)(_nextx-_beginx);
}
double Edge::corry(double x)const{
	return ((double)(x-_beginx)*slope() + (double)_beginy);
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
/*
bool Edge::operator <(const Edge* & e) const{
	return corry()<e->corry();
}
*/

Point::Point(Edge* e1, Edge* e2, double x, int type)
		:_x(x),_type(type){

	if(type == 3){
		if(e1->slope() < e2->slope())
			swap(e1,e2);
	}
	e[0]=e1;
	e[1]=e2;
}

bool PolyMgr::CrossorNot(Edge* e1, Edge* e2, double& x){
//	cout<<"judging"<<endl;
	if(e1 == 0 || e2 == 0)
		return false;
	int delta[3] ={e1->eq(0)*e2->eq(1) - e2->eq(0)*e1->eq(1),
						e1->eq(2)*e2->eq(1) - e2->eq(2)*e1->eq(1),
						e1->eq(0)*e2->eq(2) - e2->eq(0)*e1->eq(2)};
	if(delta[0]!=0){
		//cerr<<"x"<<(double)delta[1]/(double)delta[0] <<endl;
		if(x > (double)delta[1]/(double)delta[0])
			return false;
		x = (double)delta[1]/(double)delta[0];
		if(x <= e1->_beginx || x<= e2->_beginx )
			return false;
		if(x >= e1->_nextx || x>= e2->_nextx )
			return false;
		
		return true;
	}else{
		return false;
		if(delta[1]==0 && delta[2]==0){
			x = (e1->_nextx < e2->_nextx)?e1->_nextx:e2->_nextx;
			return true;
		}else
			return false;
			
	}
		
}

void PolyMgr::check_new_cross(set<Point*,PointComp>& points,
set<Edge*,EdgeComp>& edges, Edge* e, double x_now){
		double crossx = x_now;
		Edge* crosse;
		Point* nextp;
//		EdgeComp::_pretime = 1;
		if(edges.find(e) != edges.begin()){
			crosse = *(--edges.find(e));
			if(CrossorNot(crosse,e,crossx)){
				nextp = new Point(crosse,e,crossx,3);
//				PointComp p;
//				p(nextp,nextp);
				if(*points.begin()<nextp)	
					points.insert(nextp);
			}
		}

		crossx = x_now;

		if((++edges.find(e)) != edges.end()){
			crosse = *(++edges.find(e));
			if(CrossorNot(crosse,e,crossx)){
				nextp = new Point(crosse,e,crossx,3);	
//				PointComp p;
//				if(p(*points.begin(),nextp))
				if(*points.begin()<nextp)	
				points.insert(nextp);
			}
		}
//		EdgeComp::_pretime = 0;
}

double PolyMgr::find_density_inside(int x1,int x2,int y1,int y2){
	set<Point*,PointComp> points;
	set<Edge*,EdgeComp> edges;
	for(int i=0;i<_polygons.size();i++){
		for(int j=0;j<_polygons[i]->_ups.size();j++){
		Point* newp = new Point(_polygons[i]->_ups[j],_polygons[i]->_downs[j],
_polygons[i]->_ups[j]->_beginx,0);
		points.insert(newp);
		}
	}

	double x_prev = (*points.begin())->_x;
	double x_now;
	double area;
	double length_prev=0;
	double length_now=0;

	while(!points.empty()){
		Point* nowp = *points.begin();
		x_now = nowp->_x;
		Edge::_scanx = x_now;		

		length_now = findlength(&edges);
		if(x_now != x_prev)
			area+=(length_now+length_prev)*(x_now-x_prev)/2;
		//cerr<<"length_prev"<< length_prev<<endl;
		//cerr<<"length_now"<< length_now<<endl;
		if(nowp->_type == 0){//begin point
			//cerr<<"beginpoint"<<endl;
			for(int i=0;i<2;i++){
				edges.insert(nowp->e[i]);
				int type;
				type = (nowp->e[i]->_next == 0)?-1:1;
				Point* nextp = new Point(nowp->e[i],nowp->e[i]->_next,
										nowp->e[i]->_nextx,type);
				points.insert(nextp);
				check_new_cross(points,edges,nowp->e[i],x_now);
			}	
			length_prev = findlength(&edges);
		}else if(nowp-> _type == 1){//mid point
			//cerr<<"midpoint"<<endl;
			edges.erase(nowp->e[0]);
			edges.insert(nowp->e[1]);
			length_prev = length_now;
			int type;
			type = (nowp->e[1]->_next == 0)?-1:1;
			Point* nextp = new Point(nowp->e[1],nowp->e[1]->_next,
						nowp->e[1]->_nextx,type);
			points.insert(nextp);
			
			check_new_cross(points,edges,nowp->e[1],x_now);
		}else if(nowp-> _type == -1){//end point
			//cerr<<"endpoint"<<endl;
//			if(x_now != x_prev){
//			}
			Edge* crosse1=0;
			Edge* crosse2=0;
			if(edges.find(nowp->e[0]) != edges.begin())
				crosse1 = *(--edges.find(nowp->e[0]));
			if(++edges.find(nowp->e[0]) != edges.end())
				crosse2 = *(++edges.find(nowp->e[0]));

		//cerr<<"--size"<< edges.size()<<endl;
			EdgeComp::_pretime = 1;
			edges.erase(nowp->e[0]);
			EdgeComp::_pretime = 0;
		//cerr<<"--size"<< edges.size()<<endl;
			
			double crossx = x_now;
			Point* nextp;
			if(CrossorNot(crosse1,crosse2,crossx)){
				nextp = new Point(crosse1,crosse2,crossx,3);	
				points.insert(nextp);
			}
			length_prev = findlength(&edges);

		}else if(nowp-> _type == 3){//cross point
			//cerr<<"crosspoint"<<endl;
			EdgeComp::_pretime = 1;
			edges.erase(nowp->e[0]);
			EdgeComp::_pretime = 0;
			edges.insert(nowp->e[0]);
			length_prev = length_now;
			for(int i=0;i<2;i++){
				//cout<<i<<" "<<nowp->e[i]->_beginy<<endl;
				check_new_cross(points,edges,nowp->e[i],x_now);
			}
		}
		//cerr<<"size"<< edges.size()<<endl;
		

		points.erase(points.begin());	
		x_prev = x_now;
	}
	return area;
}

double PolyMgr::findlength(set<Edge*,EdgeComp>* edges){
	int a = 0;
	double length =0;
	double start;
	for(set<Edge*,EdgeComp>::iterator it=edges->begin();it!=edges->end();++it){
		//cerr<<(*it)->_beginy<<"\t";

		if(a==0)
			start = (*it)->corry();
		if(!((*it)->_up_or_down))
			a++;
		else
			a--;
		if(a==0)
			length+= start - (*it)->corry();
	}
	//cerr<<endl;
	return length;
}
