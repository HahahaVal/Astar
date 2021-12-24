#ifndef __MAP_UTIL_HPP_
#define __MAP_UTIL_HPP_
#include <stdio.h>
#include "Astar.h"

#define MALLOC malloc
#define FREE free

extern "C" {
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

enum MarkType
{
    WALKABLE = 0,
    UNWALKABLE =  1,
};

enum class FindPathRet {
    FAILURE = 0,
    SUCCESS = 1,
};

struct GridData {
    GridData() {mark = UNWALKABLE;}
    //注意字节对齐
    unsigned char mark;
};


class GridMapData {
public:
    GridMapData(int _mapId):w(0),h(0),memw(0),memh(0),mapData(nullptr),mapId(_mapId) {}

    bool operator()(const int &x,const int &y)const;
    bool init(lua_State *L);
    
    void checkPos(lua_State *L, int x, int y);
    
    unsigned int w, h;
    unsigned int memw, memh;

    GridData **mapData;
    ~GridMapData();
private:
    int mapId;
};

struct UD_GripMap{
    struct GridMapData *mapData;//地图原始数据
    struct GridMapData *mapDataReal;//地图数据副本
};

GridMapData *getGridMapData(lua_State *L, int mapId);
GridMapData *loadDataFromLua(lua_State *L, int mapId);
void unloadGripMap(int mapId);
void copyGridMapData(GridMapData *newMapData, GridMapData *oldMapData);
extern const char *LUA_LOAD_FUNC;

#endif