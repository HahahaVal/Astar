#include "MapUtil.h"
#include <stdlib.h>
#include <assert.h>

extern "C" {
    int luaopen_AstarCore(struct lua_State *L);
}

#define IS_ORIGINAL_MAP_DATA(udGripMap) (udGripMap->mapDataSrc == udGripMap->mapDataCopy)
#define GET_UD_PTR(i) struct UD_GripMap *udGripMap = (struct UD_GripMap *)lua_touserdata(L, i);assert(udGripMap);

//Aster.RegLoadMapFunc(func)
static int lregloadmapfunc(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TFUNCTION);
    lua_setfield(L, LUA_REGISTRYINDEX, LUA_LOAD_FUNC);
    return 0;
}

//Aster.UnloadGripMap(mapId)
static int lunloadGripMap(lua_State *L)
{
    int mapId = luaL_checkinteger(L, 1);
    unloadGripMap(mapId);
    return 0;
}

static int lgcUDMap(lua_State *L)
{
    GET_UD_PTR(1);
    if(!IS_ORIGINAL_MAP_DATA(udGripMap)){
        assert(udGripMap->mapDataCopy->mapData != udGripMap->mapDataSrc->mapData);
        delete udGripMap->mapDataCopy;
        udGripMap->mapDataCopy = nullptr;
        printf("udGripMap gc\n");
    }
    return 0;
}

//Astar.Create(mapId)
static int lcreate(lua_State *L)
{
    int mapId = luaL_checkinteger(L, 1);
    GridMapData *mapData = getGridMapData(L, mapId);
    if(!mapData)
    {
        return 0;
    }
    struct UD_GripMap *udGripMap = (struct UD_GripMap*)lua_newuserdata(L, sizeof(struct UD_GripMap));
    udGripMap->mapDataSrc = mapData;
    udGripMap->mapDataCopy = mapData;
    if (luaL_newmetatable(L, "UD_GripMap_mt"))
    {
        lua_pushcfunction(L, lgcUDMap);
        lua_setfield(L, -2, "__gc");
    }
    lua_setmetatable(L, -2);
    return 1;
}

//Astar.SetGripGripMark(gridData,x,y,mark)
static int lsetGripMark(lua_State *L)
{   
    GET_UD_PTR(1);
    unsigned int x = luaL_checkinteger(L, 2);
    unsigned int y = luaL_checkinteger(L, 3);
    struct GridMapData *mapDataCopy = udGripMap->mapDataCopy;
    mapDataCopy->checkPos(L, x, y);

    unsigned char mark = luaL_checkinteger(L, 4);
    if (mark > 255)
    {
        luaL_error(L, "mark is a char type,must less than 256!");
    }
    if (IS_ORIGINAL_MAP_DATA(udGripMap))
    {
        struct GridMapData *newMapData = new GridMapData(*udGripMap->mapDataSrc);
        copyGridMapData(newMapData, udGripMap->mapDataSrc);
        udGripMap->mapDataCopy = newMapData;
    }
    udGripMap->mapDataCopy->mapData[x][y].mark = mark;
    return 0;
}

//Astar.FindPath(gridData,fromX,fromY,toX,toY)
static int lfindPath(lua_State *L)
{
    GET_UD_PTR(1);
    unsigned int fromX = luaL_checkinteger(L, 2);
    unsigned int fromY = luaL_checkinteger(L, 3);
    unsigned int toX = luaL_checkinteger(L, 4);
    unsigned int toY = luaL_checkinteger(L, 5);
    struct GridMapData *mapDataCopy = udGripMap->mapDataCopy;
    mapDataCopy->checkPos(L, fromX, fromY);
    mapDataCopy->checkPos(L, toX, toY);

    int mark = mapDataCopy->mapData[toX][toY].mark;
    if(mark == UNWALKABLE)
    {
        //目标点不可走
        lua_pushinteger(L, static_cast<int>(FindPathRet::FAILURE));
        return 1;
    }

    PathVector path;
    Point start(fromX, fromY);
    Point end(toX, toY);
    Searcher<GridMapData> searcher(*mapDataCopy);
    bool ret = searcher.find_path(path, start, end);
    if (!ret) 
    {   
        //寻路失败
        lua_pushinteger(L, static_cast<int>(FindPathRet::FAILURE));
        return 1;
    }

    //寻路成功
    lua_pushinteger(L, static_cast<int>(FindPathRet::SUCCESS));
    int i = 1;
    lua_createtable(L, path.size(), 0);
    for (auto &node:path)
    {
        lua_createtable(L, 0, 2);
        lua_pushinteger(L, node.x);
        lua_setfield(L, -2, "x");
        lua_pushinteger(L, node.y);
        lua_setfield(L, -2, "y");
        lua_seti(L, -2, i++);
    }
    return 2;
}

//Astar.HasBarrier(gridData,fromX,fromY,toX,toY)
static int lhasBarrier(lua_State *L)
{
    GET_UD_PTR(1);
    unsigned int fromX = luaL_checkinteger(L, 2);
    unsigned int fromY = luaL_checkinteger(L, 3);
    unsigned int toX = luaL_checkinteger(L, 4);
    unsigned int toY = luaL_checkinteger(L, 5);
    struct GridMapData *mapDataCopy = udGripMap->mapDataCopy;
    mapDataCopy->checkPos(L, fromX, fromY);
    mapDataCopy->checkPos(L, toX, toY);

    bool rlt = mapDataCopy->hasBarrier(fromX, fromY, toX, toY);
    lua_pushboolean(L, rlt);
    return 1;
}

int luaopen_AstarCore(lua_State *L) {
    luaL_checkversion(L);
    
    luaL_Reg l[] = {
        //jps
        {"RegLoadMapFunc", lregloadmapfunc},
        {"UnloadGripMap",lunloadGripMap},
        {"Create",lcreate},
        {"SetGripGripMark",lsetGripMark},
        {"FindPath", lfindPath},
        {"HasBarrier", lhasBarrier},
        {NULL,NULL},
    };
    
    luaL_newlib(L,l);
    return 1;
}