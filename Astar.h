#ifndef __Astar_H__
#define __Astar_H__

#include <iostream>
#include <vector>
using namespace std;

#define DISTANCE 10

typedef enum
{
    UNKNOWN = 0,    //初始
    OPEN = 1,       //待处理
    CLOSE = 2,      //已处理
    BARRIER =3     //障碍物
}Flag;

class Point
{
public:
    int x;
    int y;

    int f;  //路径评分  f = g+h
    int g;  //当前点到起点的真实距离,前一个节点的g值加上两节点之间的距离
    int h;  //当前节点到终点的“估算距离
    Flag flag;
    Point *parent;
    bool operator == (const Point point)
    {
        if (x == point.x && y == point.y)
        {
            return true;
        }
        return false;
    }

    Point(int x, int y);
    ~Point(){};
};

class MapManager
{
public:
    vector<Point *> open_list;    //待处理列表
    vector< vector<Point*> > all_points;    //所有的点

    Point *findpath(Point *begin_point, Point *end_point);

    MapManager(vector< vector<Point*> > &all_points, int max_x, int max_y);
    ~MapManager();

private:
    int max_x;
    int max_y;
    int getf(Point *point, Point *end_point);
    int geth(Point *point, Point *end_point);

    vector<Point*> get_neighboring_point(Point* point);
};

#endif