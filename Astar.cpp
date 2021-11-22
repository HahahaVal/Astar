#include "Astar.h"
#include <algorithm>

bool fsort(const Point *p1,const Point *p2)
{
    return p1->f < p2->f;
}

Point::Point(int _x, int _y)
{
    x = _x;
    y = _y;
    f = 0;
    g = 0;
    h = 0;
    flag = UNKNOWN;
    parent = NULL;
}

MapManager::MapManager(vector< vector<Point*> > &_all_points, int _max_x, int _max_y)
{
    all_points = _all_points;
    max_x = _max_x;
    max_y = _max_y;
}

MapManager::~MapManager()
{
    open_list.clear();
    all_points.clear();
}

int MapManager::getf(Point *point, Point *end_point)
{
    return (point->g + geth(point, end_point));
}

int MapManager::geth(Point *point, Point *end_point)
{
    //曼哈顿距离
    return (abs(end_point->y - point->y) + abs(end_point->x - point->x))*DISTANCE;
}

Point *MapManager::findpath(Point *begin_point, Point *end_point)
{
    if (end_point->flag == BARRIER)
    {
        cout << "终点是障碍物"<<endl;
        return NULL;
    }
    if (*begin_point == *end_point)
    {
        cout<<"起点和终点相同"<<endl;
        return NULL;
    }

    begin_point->flag = OPEN;
    begin_point->h = geth(begin_point, end_point);
    begin_point->f = getf(begin_point, end_point);
    open_list.push_back(begin_point);

    //A*查找
    do 
    {
        //获取最小的节点
        Point *cur_point = open_list[0];
        cur_point->flag = CLOSE;
        open_list.erase(open_list.begin());

        if ( *cur_point == *end_point )
        {   
            return cur_point;
        }

        //获取当前节点相邻的节点
        vector<Point*> near_points = get_neighboring_point(cur_point);
        for( int i=0; i<near_points.size(); i++)
        {
            Point *tmp_point = near_points[i];

            if (tmp_point->flag == CLOSE)
            {
                continue;
            }
            else if (tmp_point->flag == UNKNOWN)
            {
                tmp_point->g = cur_point->g + DISTANCE;
                tmp_point->h = geth(tmp_point, end_point);
                tmp_point->f = getf(tmp_point, end_point);
                tmp_point->parent = cur_point;
                tmp_point->flag = OPEN;

                open_list.push_back(tmp_point);
            }
            else if (tmp_point->flag == OPEN)
            {
                if (tmp_point->g > cur_point->g + DISTANCE)
                {
                    tmp_point->parent = cur_point;
                    tmp_point->g = cur_point->g + DISTANCE;
                }
            }
        }
        //排序
        sort(open_list.begin(), open_list.end(), fsort);

    } while (open_list.size()>0);

    return NULL;
}

//查找相邻向上下左右一个节点
vector<Point*> MapManager::get_neighboring_point(Point* point)
{
    vector<Point*> neighbour_list;
    if (point->x < max_x-1)
    {
        if (all_points[point->x+1][point->y]->flag != BARRIER)
        {
            neighbour_list.push_back(all_points[point->x+1][point->y]);
        }
    }
    if (point->x >0)
    {
        if (all_points[point->x-1][point->y]->flag != BARRIER)
        {
            neighbour_list.push_back(all_points[point->x-1][point->y]);
        }
    }
    if (point->y < max_y-1)
    {
        if (all_points[point->x][point->y+1]->flag != BARRIER)
        {
            neighbour_list.push_back(all_points[point->x][point->y+1]);
        }
    }
    if (point->y >0)
    {
        if (all_points[point->x][point->y-1]->flag != BARRIER)
        {
            neighbour_list.push_back(all_points[point->x][point->y-1]);
        }
    }

    return neighbour_list;
}