#include "MapUtil.h"
#include <mutex>

#define MAX std::max
#define MIN std::min

static std::mutex gLock;
#define LOCK gLock.lock()
#define UNLOCK gLock.unlock()
#define BOUNDARY 999999

typedef std::map<int,GridMapData*> GridMapDataDict;
static GridMapDataDict G_GRID_MAP_DICT;
const char *LUA_LOAD_FUNC = "LUA_LOAD_FUNC";

GridMapData *getGridMapData(lua_State *L, int mapId)
{
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
    unsigned int memw = newMapData->memw;
    unsigned int memh = newMapData->memh;
    newMapData->mapData = (GridData**)MALLOC(sizeof(*newMapData->mapData) * memw);
    for (unsigned int x=0; x<memw; ++x)
    {
        GridData *yGrid = new GridData[memh];
        newMapData->mapData[x] = yGrid;
        GridData *orginal = oldMapData->mapData[x];
        for(unsigned int y = 0; y < memh; ++y){
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

/*获取直接与矩形关系
@params k:直线斜率
@params b:x为0时的偏移
@params gridX gridY: 格子坐标

@return 0:相交; 1:直线在矩形上方; -1:直线在矩形下方
*/
int GridMapData::getLineRelation(float k, float b, int gridX, int gridY)
{
    if (k > 0)
    {
        //比较矩形左下两条线
        float x1 = gridX*gridW;
        float y1 = (gridY + 1)*gridH;
        
        float value = k*x1 + b;
        if (value >= y1)
            return 1;
        
        float x2 = (gridX+1)*gridW;
        float y2 = gridY*gridH;
        value = k*x2 + b;
        if (value <= y2)
            return -1;
        
        return 0;
    }
    else
    {
        //比较矩形右上两条线
        float x1 = gridX*gridW;
        float y1 = gridY*gridH;
        
        float value = k*x1 + b;
        if (value <= y1)
            return -1;
        
        float x2 = (gridX+1)*gridW;
        float y2 = (gridY+1)*gridH;
        value = k*x2 + b;
        
        if (value >= y2)
            return 1;
            
        return 0;
    }
}

bool GridMapData::hasBarrier(int fromX, int fromY, int toX, int toY)
{
    // 垂直
    if (fromX == toX)
    {
        int minY = MIN(fromY,toY);
        int maxY = MAX(fromY,toY);
        for (int y = minY; y <= maxY; y++)
        {
            if (mapData[fromX][y].mark == UNWALKABLE)
            {
                return true;
            }
        }
    }
    // 水平
    else if (fromY == toY)
    {
        int minX = MIN(fromX,toX);
        int maxX = MAX(fromX,toX);
        for (int x = minX; x <= maxX; x++)
        {
            if (mapData[x][fromY].mark == UNWALKABLE)
            {
                return true;
            }
        }
    }
    // 斜向
    else
    {
        int dx = toX - fromX;
        int dy = toY - fromY;

        float k = (float)dy*gridH/((float)dx*gridW);
        float b = (fromY+0.5)*gridH - k*(fromX+0.5)*gridW;
        
        int minX = MIN(fromX,toX);
        int maxX = MAX(fromX,toX);
        
        int minY = MIN(fromY,toY);
        int maxY = MAX(fromY,toY);
        
        int boundaryX1 = 0;
        int boundaryY1 = BOUNDARY;
        
        int boundaryX2 = BOUNDARY;
        int boundaryY2 = 0;
        
        if (k < 0)
        {
            boundaryY1 = 0;
            boundaryY2 = BOUNDARY;
        }
        
        for (int x = minX; x <= maxX; x++)
        {
            for (int y = minY; y <= maxY; y++)
            {
                // 边界过滤
                if (k > 0)
                {
                    if (x <= boundaryX1 && y >= boundaryY1)
                        continue;

                    if (x >= boundaryX2 && y <= boundaryY2)
                        continue;
                }
                else
                {
                    if (x <= boundaryX1 && y <= boundaryY1)
                        continue;

                    if (x >= boundaryX2 && y >= boundaryY2)
                        continue;
                }

                // 线与矩形的关系
                int relation = getLineRelation(k, b, x, y);
                // 相交
                if (relation == 0)
                {
                    if (mapData[x][y].mark == UNWALKABLE)
                    {
                        return true;
                    }                        
                    
                    if (x == toX && y == toY)
                        continue;

                    //相邻的两个格子都不可走，则被夹在中间的格子也应该不可走
                    int x1 = x;
                    int y1 = y+(dy>0 ? 1:-1);
                    
                    if (x1 < 1 || x1 > w)
                        continue;
                    
                    if (y1 < 1 || y1 > h)
                        continue;
                    
                    if (mapData[x1][y1].mark == UNWALKABLE)
                    {
                        int x2 = x+(dx>0 ? 1:-1);
                        int y2 = y;
                        
                        if (x2 < 1 || x2 > w)
                            continue;
                        
                        if (y2 < 1 || y2 > h)
                            continue;
                        
                        if (mapData[x2][y2].mark == UNWALKABLE)
                        {
                            return true;
                        }                            
                    }
                }
                // 直线在矩形上方
                else if (relation > 0)
                {
                    if (k > 0)
                    {
                        boundaryX2 = x;
                        boundaryY2 = y;
                    }
                    else
                    {
                        boundaryX1 = x;
                        boundaryY1 = y;
                    }
                }
                 // 直线在矩形下方
                else if (relation < 0)
                {
                    if (k > 0)
                    {
                        boundaryX1 = x;
                        boundaryY1 = y;
                    }
                    else
                    {
                        boundaryX2 = x;
                        boundaryY2 = y;
                    }
                }
            }
        }
    }
    return false;
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