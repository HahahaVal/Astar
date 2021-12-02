#ifndef __MAP_UTIL_HPP_
#define __MAP_UTIL_HPP_
#include "Astar.h"

enum MarkType
{
    WALKABLE = 0,
    UNWALKABLE =  1,
};


class GridData {
public:
    GridData() {mark = UNWALKABLE;}

    int mark;
    ~GridData() {}
};


class GridMapData {
public:
    GridMapData(int _mapId):mapId(_mapId){}

    int blockType;//格挡类型。

    bool operator()(const int &x,const int &y)const;
    bool init(int **array, int w, int h);
    ~GridMapData() {};

private:
    int mapId;
    int w, h;
    GridData **mapData;
};

#endif