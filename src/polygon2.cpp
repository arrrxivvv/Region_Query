#include"polygon.h"
using namespace std;
#define inf 10000
#include <stdlib.h>

vector<Polygon*> PolyMgr::find_interact(int x1, int y1, int x2, int y2)
{
        vector<Polygon*> total;
//      cout << "(x1,y1)=(" << x1 << "," << y1 << ") (x2,y2)=(" << x2 << "," << y2 << ")" << endl;
        for(int i=0; i<_polygons.size(); i++)
        {
                int p_x1 = _polygons[i]->_Vertices[_polygons[i]->_leftIndex].x;
                int p_y1 = _polygons[i]->_Vertices[_polygons[i]->_lowIndex].y;
                int p_x2 = _polygons[i]->_Vertices[_polygons[i]->_rightIndex].x;
                int p_y2 = _polygons[i]->_Vertices[_polygons[i]->_highIndex].y;
                if(p_x1>=x1 && p_y1>=y1 && p_x2<=x2 && p_y2<=y2)
                {
                        total.push_back(_polygons[i]);
//                      cout << "windows contains whole polygon" << endl;
                }
                else if( p_x1<=x2 && p_x2>=x1 && p_y1<=y2 && p_y2>=y1 )
                {
                        int w_x1 = max(x1,p_x1);
                        int w_y1 = max(y1,p_y1);
                        int w_x2 = min(x2,p_x2);
                        int w_y2 = min(y2,p_y2);
//                      cout << "(w_x1,w_y1)=(" << w_x1 << "," << w_y1 << ") (w_x2,w_y2)=(" << w_x2 << "," << w_y2 << ")" << endl;

                        vector<Cross> xcross;
                        bool is_v=false;
                        bool is_cross=false;
                        for(int j=0; j<_polygons[i]->_Vertices.size(); j++)
                        {
                                int _beginx = _polygons[i]->_Vertices[j].x;
                                int _beginy = _polygons[i]->_Vertices[j].y;
                                int _endx;
                                int _endy;

                                if(j != _polygons[i]->_Vertices.size()-1)
                                {
                                        _endx = _polygons[i]->_Vertices[j+1].x;
                                        _endy = _polygons[i]->_Vertices[j+1].y;
                                }
                                else
                                {
                                        _endx = _polygons[i]->_Vertices[0].x;
                                        _endy = _polygons[i]->_Vertices[0].y;
                                }
                                if( !((_beginx>w_x2 && _endx>w_x2) || (_beginx<w_x1 && _endx<w_x1)) )
                                {
                                        if(_beginx==_endx)
                                        {
                                                if(!((_beginy>w_y2 && _endy>w_y2) || (_beginy<w_y1 && _endy<w_y1)))
                                                {
                                                        is_cross = true;
                                                        break;
                                                }
                                                if(_beginx==w_x1)
                                                {
                                                        if(j==_polygons[i]->_Vertices.size()-1)
                                                        {
                                                                xcross[0].is_vertical=true;
                                                        }
                                                        else
                                                        {
                                                                Cross tmp2={_endy,true,true,false};
                                                                xcross.push_back(tmp2);
                                                        }
                                                        Cross tmp1={_beginy,true,true,false};
                                                        xcross.push_back(tmp1);
                                                        is_v = true;
                                                }
                                        }
                                        else
                                        {
                                                int start_x;
                                                int finish_x;
                                                double m = ((double)_endy-(double)_beginy)/((double)_endx-(double)_beginx);
                                                if( _beginx>=w_x1 && _beginx<=w_x2 && _endx>=w_x1 && _endx<=w_x2 )
                                                {
                                                        if(!((_beginy>w_y2 && _endy>w_y2) || (_beginy<w_y1 && _endy<w_y1)))
                                                        {
                                                                is_cross = true;
                                                                break;
                                                        }
                                                        start_x = min(_beginx,_endx);
                                                        finish_x = max(_beginx,_endx);
                                                }
                                                else if(_beginx>=w_x1 && _beginx<=w_x2)
                                                {
                                                        if(_endx>w_x2)
                                                        {
                                                                start_x = _beginx;
                                                                finish_x = w_x2;
                                                        }
                                                        else if(_endx<w_x1)
                                                        {
                                                                start_x = w_x1;
                                                                finish_x = _beginx;
                                                        }
                                                }
                                                else if(_endx>=w_x1 && _endx<=w_x2)
                                                {
                                                        if(_beginx>w_x2)
                                                        {
                                                                start_x = _endx;
                                                                finish_x = w_x2;
                                                        }
                                                        else if(_beginx<w_x1)
                                                        {
                                                                start_x = w_x1;
                                                                finish_x = _endx;
                                                        }
                                                }
                                                else
                                                {
                                                        start_x = w_x1;
                                                        finish_x = w_x2;
                                                }

                                                double cy1 = (double)_beginy + m*((double)start_x-(double)_beginx);
                                                double cy2 = (double)_beginy + m*((double)start_x-(double)_beginx);
                                                if(!((cy1>w_y2 && cy2>w_y2) || (cy1<w_y1 && cy2<w_y1)))
                                                {
                                                        is_cross = true;
                                                        break;
                                                }

                                                if(start_x==w_x1)
                                                {
                                                        double y = (double)_beginy + m*((double)start_x-(double)_beginx);
                                                        int kk = j-1;
                                                        if(start_x==_beginx && !is_v)
                                                        {
                                                                if(j==0) kk = _polygons[i]->_Vertices.size()-1;
                                                                if((_endx-_beginx)*( _polygons[i]->_Vertices[kk].x-_beginx)<=0)
                                                                {
                                                                        Cross tmp1={y,true,false,false};
                                                                        xcross.push_back(tmp1);
                                                                }
                                                                else
                                                                {
                                                                        Cross tmp1={y,true,false,true};
                                                                        xcross.push_back(tmp1);
                                                                        xcross.push_back(tmp1);
                                                                }
                                                        }
                                                        else if(start_x==_beginx && is_v)
                                                        {
                                                                is_v = false;
                                                        }
                                                        else if(start_x==_endx){}
                                                        else
                                                        {
                                                                Cross tmp2={y,false,false,false};
                                                                xcross.push_back(tmp2);
                                                        }
                                                }

                                        }
                                }
                        }

                        if(is_cross)
                        {
                                total.push_back(_polygons[i]);
                                continue;
                        }

                        for(int w=1; w<xcross.size(); w++)
                                        //                                                                                            242,3-24      42%
                        {
                                Cross key = xcross[w];
                                int l = w-1;
                                while(l>=0 && xcross[l].y<key.y)
                                {
                                        xcross[l+1] = xcross[l];
                                                l = l-1;
                                }
                                xcross[l+1] = key;
                        }

                        int w=0;
                        while(w<xcross.size()-1)
                        {
                                if(xcross[w].is_vertical && xcross[w+1].is_vertical && w%2==1)
                                {
                                        vector<Cross> tmp;
                                        Cross v1 = xcross[w];
                                        Cross v2 = xcross[w+1];
                                        for(int l=0; l<=w; l++)
                                        {
                                                tmp.push_back(xcross[l]);
                                        }
                                        tmp.push_back(v1);
                                        tmp.push_back(v2);
                                        tmp.push_back(v2);
                                        for(int l=w+2; l<xcross.size(); l++)
                                        {
                                                tmp.push_back(xcross[l]);
                                        }
                                        xcross.erase (xcross.begin(),xcross.end());
                                        for(int q=0; q<tmp.size(); q++)
                                        {
                                                xcross.push_back(tmp[q]);
                                        }
                                        w = w+3;
                                }
                                                                                                                                    //279,3-24      49%
                                w = w+1;
                        }

                                for(int w=0; w<xcross.size(); w++)
                                {
                                        //cout << "(x,y)=(" << w_x1 << "," << xcross[w].y << ")  is_vertex=" << xcross[w].is_vertex;
                                        //cout << "  is_vertical=" << xcross[w].is_vertical << "  sharp_vertex=" << xcross[w].sharp_vertex<<endl;
                                }

                        if( !((xcross[0].y < w_y1) || (xcross[xcross.size()-1].y > w_y2)) )
                        {
                                for(int w=0; w<xcross.size(); w=w+2)
                                {
                                        if( (xcross[w].y<=w_y2 && xcross[w].y>=w_y1) || (xcross[w+1].y<=w_y2 && xcross[w+1].y>=w_y1) )
                                        {
                                                total.push_back(_polygons[i]);
                                                break;
                                        }
                                        else if( xcross[w].y>w_y2 && xcross[w+1].y<w_y1)
                                        {
                                                total.push_back(_polygons[i]);
                                                break;
                                        }
                                }
                        }

                }
        }
        return total;
}//end problem 2

