#include"polygon.h"
using namespace std;
#define inf 10000
#include <stdlib.h>


int PolyMgr::find_inside(int x1, int y1, int x2, int y2)
{
	int sum = 0;
	for(int i=0; i<_polygons.size(); i++)
	{
		Polygon* s1=_polygons[i];
		if(s1->_Vertices[s1->_leftIndex].x>=x1 && s1->_Vertices[s1->_rightIndex].x<=x2 && s1->_Vertices[s1->_highIndex].y<=y2 && s1->_Vertices[s1->_lowIndex].y>=y1)
		sum = sum+1;
	}
	return sum;

}//end problem 1
