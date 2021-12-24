#include "MapUtil.h"
#include <mutex>

static std::mutex gLock;

#define LOCK gLock.lock()
#define UNLOCK gLock.unlock()

typedef std::map<int,GridMapData*> GridMapDataDict;
static GridMapDataDict G_GRID_MAP_DICT;
const char *LUA_LOAD_FUNC = "LUA_LOAD_FUNC";

GridMapData *getGridMapData(lua_State *L, int mapId){
    GridMapData *data = nullptr;
    LOCK;
    GridMapDataDict::const_iterator it = G_GRID_MAP_DICT.find(mapId);
    if(it != G_GRID_MAP_DICT.end()) 
	{
        data = it->second;
    }
    UNLOCK;
    if(!data){
        data = loadDataFromLua(L,mapId);
    }
    return data;
}

GridMapData *loadDataFromLua(lua_State *L, int mapId)
{
    lua_getfield(L, LUA_REGISTRYINDEX, LUA_LOAD_FUNC);
    if(lua_type(L, -1) != LUA_TFUNCTION )
    {
        luaL_error(L, "load grid map data function not exist!");
        return nullptr;
    }
    lua_pushinteger(L, mapId);
    lua_call(L, 1, 1);

    //加载函数返回一个2维的地图数组
    GridMapData *data = new GridMapData(mapId);
    if(!data->init(L)){
        delete data;
        data = nullptr;
        luaL_error(L, "grid data invalidate");
        return nullptr;
    }
    LOCK;
    G_GRID_MAP_DICT[mapId] = data;
    UNLOCK;
    return data;
}

void unloadGripMap(int mapId)
{
    LOCK;
    GridMapData *data = G_GRID_MAP_DICT[mapId];
    delete data;
    G_GRID_MAP_DICT.erase(mapId);
    UNLOCK;
}

void copyGridMapData(GridMapData *newMapData, GridMapData *oldMapData)
{
    newMapData->mapData = (GridData**)MALLOC(sizeof(*newMapData->mapData) * newMapData->memw);
    for (unsigned int x=0; x<newMapData->memw; ++x)
    {
        GridData *yGrid = new GridData[newMapData->memh];
        newMapData->mapData[x] = yGrid;
        GridData *orginal = oldMapData->mapData[x];
        for(unsigned int y = 0; y < newMapData->memh; ++y){
            yGrid[y] = orginal[y];
        }
    }
}

bool GridMapData::operator()(const int &x, const int &y)const
{
	if( x < 1 || x > w || y < 1 || y > h)
		return false;
	return mapData[x][y].mark == WALKABLE;
}

bool GridMapData::init(lua_State *L)
{
    if (lua_type(L, -1) != LUA_TTABLE)
    {
        luaL_error(L, "load grid data is'n table!");
        return false;
    }
	w = luaL_len(L, -1);
    memw = w+2;
	lua_geti(L, -1, 1);
	if(lua_type(L, -1) != LUA_TTABLE){
        luaL_error(L, "load grid data failded!");
        return false;
    }
	h = luaL_len(L, -1);
    memh = h+2;
	lua_pop(L, 1); 
    printf("map init w:%d,h:%d\n",w,h);

    //init mapData
	mapData = (GridData **)MALLOC(sizeof(*mapData)*memw);
	for (unsigned int i=0; i<memw; ++i) {
        GridData *yGrid = new GridData[memh];
        mapData[i] =yGrid;
    }

	for(int i = 1; i <= w; ++i){
        lua_geti(L, -1, i);
        if(lua_type(L, -1) != LUA_TTABLE)
		{
        	luaL_error(L, "load grid data failded!");
       		return false;
        }
        for (unsigned int j = 1; j <= h; ++j) {
            lua_geti(L, -1, j);
            if(lua_type(L, -1) != LUA_TTABLE)
			{
                luaL_error(L, "load grid data failded!");
       			return false;
            }
            lua_getfield(L, -1, "p");
            mapData[i][j].mark = lua_tointeger(L, -1);
            lua_pop(L, 2);
        }
        lua_pop(L, 1);
    }
	return true;
}

void GridMapData::checkPos(lua_State *L, int x, int y)
{
    if(w < x || x < 1)
        luaL_error(L,"x is over,x:%d,weiht:%d", x, w);
    if(h < y || y < 1)
        luaL_error(L,"y is over,y:%d,hight:%d", y, h);
}

GridMapData::~GridMapData(){
	if(mapData){
		for (unsigned int i = 0; i<memw; ++i) {
			GridData *yGrids = mapData[i];
			if(yGrids){
				delete[] yGrids;
			}
		}
		FREE(mapData);
	}   
}