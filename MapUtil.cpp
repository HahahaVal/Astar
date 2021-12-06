#include "MapUtil.h"
#include <stdlib.h>
#include <stdio.h>

bool GridMapData::operator()(const int &x, const int &y)const
{
    if( x < 1 || x > w || y < 1 || y > h)
        return false;
    if( blockType == 0 ){
       return mapData[x][y].mark == WALKABLE;
    }else{
        return (mapData[x][y].mark & blockType) == WALKABLE;
    }
}

bool GridMapData::init(int **array, int _w, int _h)
{
    w = _w;
    h = _h;
    mapData = (GridData**)malloc(sizeof(*mapData)*w);
    for (int i = 0; i<w; ++i)
    {
        GridData *hGrid = new GridData[h];
        mapData[i] = hGrid;
    }

    for (int i = 0; i<w; ++i)
    {
        for (int j = 0; j<h; ++j)
        {
            int p = *(*(array+i)+j);
            mapData[i][j].mark = p;
        }
    }
}

GridMapData::~GridMapData(){
    if(mapData){
        for (int i = 0; i<w; ++i) {
            GridData *yGrids = mapData[i];
            if(yGrids){
                delete[] yGrids;
            }
        }
        free(mapData);
    }   
}