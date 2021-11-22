#include <iostream>
#include "Astar.h"
#include <stdio.h>

#define MAX_X 10
#define MAX_Y 10

void printMap(char map[MAX_X][MAX_Y],int width,int height)
{
    for (int i = 0; i<width; i++)
    {
        for (int j = 0; j<height; j++)
        {
            printf("%c ",map[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, const char * argv[]) {
    //初始化地图 0代表障碍
    char mapdata[MAX_X][MAX_Y] =
    {
        {'1','0','0','1','0','1','1','1','1','1'},
        {'1','1','1','1','0','1','1','1','1','1'},
        {'0','0','0','1','0','1','1','1','1','1'},
        {'1','0','0','1','0','1','1','1','1','0'},
        {'1','1','1','1','0','1','1','1','1','1'},
        {'1','1','0','0','1','1','1','1','1','1'},
        {'1','1','1','1','1','1','1','1','1','1'},
        {'1','0','0','1','1','1','1','1','1','1'},
        {'1','1','0','0','1','1','1','1','1','1'},
        {'1','0','1','1','1','1','1','1','1','1'},
    };
    cout<<"---打印初始地图---"<<endl;
    printMap(mapdata, MAX_X, MAX_Y);
    //创建地图
    vector< vector<Point*> > map;
    for (int i = 0; i<MAX_X; i++)
    {
        vector<Point*> tmp;
        for (int j = 0; j<MAX_Y; j++)
        {
            Point *point = new Point(i, j);
            if (mapdata[i][j]=='0')
            {
                point->flag = BARRIER;
            }
            tmp.push_back(point);
        }
        map.push_back(tmp);
    }

    //开始寻路
    MapManager *manager= new MapManager(map, MAX_X, MAX_Y);
    Point *point = manager->findpath(map[0][0], map[9][9]);
   
    if (!point)
    {
        return 0;
    }
    
    cout<<"---打印寻路点---"<<endl;
    while (point)
    {
        mapdata[point->x][point->y] = '*';
        cout<<point->x<<","<<point->y<<endl;
        point = point->parent;
    }
    cout<<"---打印路径---"<<endl;
    printMap(mapdata, MAX_X, MAX_Y);
    
    delete manager;

    for (int i = 0; i<MAX_X; i++)
    {
        for (int j = 0; j<MAX_Y; j++)
        {
            delete map[i][j];
        }
    }
    return 0;
}