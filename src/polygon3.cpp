#include"polygon.h"
#include<queue>
#include<set>
#include<iostream>
using namespace std;

/*double Edge::_scanx = 0;
double Edge::slope()
{
	return (double)(_nexty-_beginy)/(double)(_nextx-_beginx);
}
double Edge::corry(double x)
{
	return ((double)(x-_beginx)*slope() + (double)_beginy);
}*/
Point::Point(Edge* e1, Edge* e2, double x, double y, int type)
		:_x(x),_y(y),_type(type)
{
	if(type == 3)
	{
		if(e1->slope() < e2->slope())  {swap(e1,e2);}    //??
	}
	e[0]=e1;
	e[1]=e2;
}



int cross(const int& p1_x,const int& p1_y,const int& p2_x,const int& p2_y,const int& p3_x,const int& p3_y)
{
	return (p2_x - p1_x)*(p3_y - p1_y) - (p2_y - p1_y)*(p3_x - p1_x);
} 

bool aux_intersect(int a1,int a2,int b1,int b2)
{
	if(a1>a2)  {swap(a1,a2);}
	if(b1>b2)  {swap(b1,b2);}
	return max(a1,b1)<=min(a2,b2);
}

bool intersect(const Edge* e1,const Edge* e2)
{
	if( !e1 || !e2 )  {return false;}
	return aux_intersect(e1->_beginx,e1->_nextx,e2->_beginx,e2->_nextx)
	    && aux_intersect(e1->_beginy,e1->_nexty,e2->_beginy,e2->_nexty)
	    && cross(e1->_beginx,e1->_beginy,e1->_nextx,e1->_nexty,e2->_beginx,e2->_beginy)
               * cross(e1->_beginx,e1->_beginy,e1->_nextx,e1->_nexty,e2->_nextx,e2->_nexty) <0
	    && cross(e2->_beginx,e2->_beginy,e2->_nextx,e2->_nexty,e1->_beginx,e1->_beginy)
               * cross(e2->_beginx,e2->_beginy,e2->_nextx,e2->_nexty,e1->_nextx,e1->_nexty) <0;
}


struct PointCMP
{
	bool operator() (const Point* p1,const Point* p2) const
	{
		if(p1->_x != p2->_x)
		{return p1->_x > p2->_x;}
		else if(p1->_y != p2->_y)
		{return p1->_y < p2->_y;}
	}
};

struct EdgeCMP
{
	bool operator() (const Edge* e1,const Edge* e2)
	{
		if( e1->corry() != e2->corry() )
		{return e1->corry() > e2->corry();}
		else if( e1->slope() != e2->slope() )
		{
			double x,y;
			intersection(e1,e2,x,y);
			if(x > e1->_scanx)
			return e1->slope() > e2->slope();
			else
			return e1->slope() < e2->slope();
		}
   	 	else
		{
			if(e1->_up_or_down == e1->_up_or_down)
			{return e1->_num < e2->_num;}
			else if(e1->_up_or_down)    //down=1
			{return true;}
			else 
			{return false;}
		}
	}
	void intersection(const Edge* e1,const Edge* e2,double &x,double &y)
	{
		x=(double)( e2->_nexty - e1->_nexty + e1->slope()*(e1->_nextx) - e2->slope()*(e2->_nextx) )	
									/ ( e1->slope() - e2->slope() );
		y=(double)( e1->slope()*(x-e1->_nextx) + e1->_nexty);
	}
};


void intersection(const Edge* e1,const Edge* e2,double &x,double &y)
{
	x=(double)( e2->_nexty - e1->_nexty + e1->slope()*(e1->_nextx) - e2->slope()*(e2->_nextx) )	
									/ ( e1->slope() - e2->slope() );
	y=(double)( e1->slope()*(x-e1->_nextx) + e1->_nexty);
}


int PolyMgr::find_no_overlap(int y1,int x1,int y2,int x2)
{
	priority_queue<Point*,vector<Point*>,PointCMP> points; 
	set<Edge*,EdgeCMP> edges;
	for(int i=0;i<_polygons.size();i++)
	{
		_polygons[i]->_flag=1;
		for(int j=0;j<_polygons[i]->_ups.size();j++)
		{
			Point* newp = new Point(_polygons[i]->_ups[j],_polygons[i]->_downs[j],
                                       _polygons[i]->_ups[j]->_beginx,_polygons[i]->_ups[j]->_beginy,0);
			points.push(newp);
		}
	}
	double x_now;
	while( !points.empty() )
	{
		Point* nowp = points.top();
		x_now = nowp->_x;
		Edge::_scanx = x_now;		
		Edge* crosse;
		if(nowp->_type == 0)//begin point
		{
//cerr<<"begin";
			for(int i=0;i<2;i++)
			{
				edges.insert(nowp->e[i]);
				int type;
				type = (nowp->e[i]->_next == 0)?-1:1;
				Point* nextp = new Point(nowp->e[i],nowp->e[i]->_next,
							nowp->e[i]->_nextx,nowp->e[i]->_nexty,type);
				points.push(nextp);
				if( edges.find(nowp->e[i])!=edges.begin() )
				{
//cerr<<"a"<<endl;
					crosse=*(--edges.find(nowp->e[i]));
if(nowp->e[i]->_parentPoly!=crosse->_parentPoly &&  !crosse->_up_or_down)
nowp->e[i]->_parentPoly->_flag=crosse->_parentPoly->_flag=0;
					if(intersect(crosse,nowp->e[i]))
					{
if(nowp->e[i]->_parentPoly!=crosse->_parentPoly)
nowp->e[i]->_parentPoly->_flag=crosse->_parentPoly->_flag=0;
						double x,y;
						intersection(crosse,nowp->e[i],x,y);
						if(x>x_now)
						{
							nextp = new Point(crosse,nowp->e[i],x,y,3);
							points.push(nextp);
//cerr<<nextp->_x<<endl;	
						}
					}
				}
				if( ++edges.find(nowp->e[i])!=edges.end() )
				{
//cerr<<"b"<<endl;
					crosse=*(++edges.find(nowp->e[i]));
if(nowp->e[i]->_parentPoly!=crosse->_parentPoly &&  crosse->_up_or_down)
nowp->e[i]->_parentPoly->_flag=crosse->_parentPoly->_flag=0;
					if(intersect(crosse,nowp->e[i]))
					{
if(nowp->e[i]->_parentPoly!=crosse->_parentPoly)
nowp->e[i]->_parentPoly->_flag=crosse->_parentPoly->_flag=0;
						double x,y;
						intersection(crosse,nowp->e[i],x,y);
						if(x>x_now)
						{
							nextp = new Point(crosse,nowp->e[i],x,y,3);
							points.push(nextp);	
//cerr<<nextp->_x<<endl;	
						}
					}
				}
			}	
		}
		else if(nowp-> _type == 1)//mid point
		{
//cerr<<"mid";
			edges.erase(nowp->e[0]);
			edges.insert(nowp->e[1]);
			int type;
			type = (nowp->e[1]->_next == 0)?-1:1;
			Point* nextp = new Point(nowp->e[1],nowp->e[1]->_next,
						nowp->e[1]->_nextx,nowp->e[1]->_nexty,type);
			points.push(nextp);			
			if( edges.find(nowp->e[1])!=edges.begin() )
			{
//cerr<<"c"<<endl;
				crosse=*(--edges.find(nowp->e[1]));
if(nowp->e[1]->_parentPoly!=crosse->_parentPoly &&  !crosse->_up_or_down)
nowp->e[1]->_parentPoly->_flag=crosse->_parentPoly->_flag=0;
				if(intersect(crosse,nowp->e[1]))
				{
if(nowp->e[1]->_parentPoly!=crosse->_parentPoly)
nowp->e[1]->_parentPoly->_flag=crosse->_parentPoly->_flag=0;
					double x,y;
					intersection(crosse,nowp->e[1],x,y);
					if(x>x_now)
					{
						nextp = new Point(crosse,nowp->e[1],x,y,3);
						points.push(nextp);	
					}
				}
			}
			if( ++edges.find(nowp->e[1])!=edges.end() )
			{
//cerr<<"d"<<endl;
				crosse=*(++edges.find(nowp->e[1]));
if(nowp->e[1]->_parentPoly!=crosse->_parentPoly &&  crosse->_up_or_down)
nowp->e[1]->_parentPoly->_flag=crosse->_parentPoly->_flag=0;
				if(intersect(crosse,nowp->e[1]))
				{
if(nowp->e[1]->_parentPoly!=crosse->_parentPoly)
nowp->e[1]->_parentPoly->_flag=crosse->_parentPoly->_flag=0;
					double x,y;
					intersection(crosse,nowp->e[1],x,y);
					if(x>x_now)
					{
						nextp = new Point(crosse,nowp->e[1],x,y,3);
						points.push(nextp);	
					}
				}
			}
		}
		else if(nowp-> _type == -1)//end point
		{
//cerr<<"end";
			Edge* crosse1=0;
			Edge* crosse2=0;
			if( edges.find(nowp->e[0])!=edges.begin() )
			{crosse1=*( --edges.find(nowp->e[0]) );}
			if( ++edges.find(nowp->e[0])!= edges.end() )
			{crosse2=*( ++edges.find(nowp->e[0]) );}
			edges.erase(nowp->e[0]);
/*	 
	cerr<<"!";
*/
			if(intersect(crosse1,crosse2))
			{
//cout<<"!";
				double x,y;
				intersection(crosse1,crosse2,x,y);
				if(x>x_now)
				{
if(crosse1->_parentPoly!=crosse2->_parentPoly)
crosse1->_parentPoly->_flag=crosse2->_parentPoly->_flag=0;
					Point* nextp = new Point(crosse1,crosse2,x,y,3);
					points.push(nextp);
				}
			}
		}
		else if(nowp-> _type == 3)//cross point
		{
//cerr<<"cross";
			edges.erase(nowp->e[0]);
			edges.erase(nowp->e[1]);
			edges.insert(nowp->e[0]);
			edges.insert(nowp->e[1]);
			Point* nextp;
			for(int i=0;i<2;i++)
			{
				if( edges.find(nowp->e[i])!=edges.begin() )
				{
//cout<<"e"<<endl;
					crosse=*(--edges.find(nowp->e[i]));
					if(intersect(crosse,nowp->e[i]))
					{
if(nowp->e[i]->_parentPoly!=crosse->_parentPoly)
nowp->e[i]->_parentPoly->_flag=crosse->_parentPoly->_flag=0;
						double x,y;
						intersection(crosse,nowp->e[i],x,y);
						if(x>x_now)
						{
							nextp = new Point(crosse,nowp->e[i],x,y,3);
							points.push(nextp);	
						}
					}
				}
				if( ++edges.find(nowp->e[i])!=edges.end() )
				{
//cerr<<"f"<<endl;
					crosse=*(++edges.find(nowp->e[i]));
					if(intersect(crosse,nowp->e[i]))
					{
if(nowp->e[i]->_parentPoly!=crosse->_parentPoly)
nowp->e[i]->_parentPoly->_flag=crosse->_parentPoly->_flag=0;
						double x,y;
						intersection(crosse,nowp->e[i],x,y);
						if(x>x_now)
						{
							nextp = new Point(crosse,nowp->e[i],x,y,3);
							points.push(nextp);	
						}
					}
				}
			}
		}
		points.pop();
	}
	int numofflag=0;
	for(int i=0;i<_polygons.size();i++)
	{
		if(_polygons[i]->_flag==1)  {numofflag++;}
	}
	return numofflag;
}
