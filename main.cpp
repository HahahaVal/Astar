#include <iostream>
#include "Astar.h"
#include "MapUtil.h"
#include <stdio.h>
#include <vector>

#define MAX_X 10
#define MAX_Y 10
using  namespace std;

void printMap(int map[MAX_X][MAX_Y],int width,int height)
{
    for (int i = 0; i<width; i++)
    {
        for (int j = 0; j<height; j++)
        {
            printf("%d ",map[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, const char * argv[]) {
    //初始化地图 1代表障碍
    int mapdata[MAX_X][MAX_Y] =
    {
        {1,1,1,1,0,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,0,1},
        {1,0,0,1,0,0,0,0,0,1},
        {1,0,0,0,1,0,0,0,0,1},
        {1,0,0,0,1,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,1},
        {1,1,1,1,0,1,1,1,1,1},
    };
    cout<<"---打印初始地图---"<<endl;
    printMap(mapdata, MAX_X, MAX_Y);

    int **p = (int**)malloc(sizeof(*p)*(MAX_X));
    if (p == NULL)
	{
		cout << "malloc failed!\n" << endl;
		return -1;
	}
    for (int i = 0; i < MAX_X; i++)
	{
		p[i] = (int *)malloc(MAX_Y * sizeof(int));
		if (p[i] == NULL)
		{
			cout << "malloc failed!\n" << endl;
			return -1;
		}
	}

	for (int i = 0; i < MAX_X; ++i)
	{
		for (int j = 0; j < MAX_Y; ++j)
		{
			p[i][j] = mapdata[i][j];
		}
	}

    //创建地图
    GridMapData gridMap = GridMapData(1001);
    gridMap.init(p, MAX_X, MAX_Y);

    //开始寻路
    PathVector path;
    Point start(2,2);
    Point end(6,6);
    Searcher<GridMapData> searcher(gridMap);
    searcher.find_path(path, start, end);

    cout<<"---打印寻路点---"<<endl;
    for (PathVector::iterator it = path.begin(); it != path.end(); ++it)
    {
        cout<< "x:" << it->x  << "  " << "y:" << it->y <<endl;
        mapdata[it->x][it->y] = 5;
    }
    cout<<"---打印路径---"<<endl;
    printMap(mapdata, MAX_X, MAX_Y);
    return 0;
}