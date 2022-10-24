// ***********************************************************************
//File            [ polygon4.cpp ]
//Author          [ Algo group 8 ]
//Synopsis        [ Implementation of 4, Find area less than ]
//Date            [  ]
//Modified From   [ Lin, without problem 1~5 implementation ]
//Last Modified   [ 2013/06/11 11:50 Lin ]
//************************************************************************
#include"polygon.h"

int PolyMgr::find_area_less_than( double maxArea)
{
	int count = 0;
	for ( int i = 0; i < _polygons.size(); ++i ){
		if ( (_polygons[i])->_area < maxArea ) {
			++count;
		}
	}
	return count;
}
