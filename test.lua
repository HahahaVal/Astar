local AstarCore = require "AstarCore"

local map = {}

map[92001] = {
    [1] = {{p=1},{p=1},{p=1},{p=1},{p=1},{p=1},{p=1},{p=1},{p=1},{p=0},{p=1},{p=1},{p=1},{p=1},{p=1},{p=1},{p=1}},
    [2] = {{p=1},{p=0},{p=1},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=1}},
    [3] = {{p=1},{p=0},{p=1},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=1}},
    [4] = {{p=1},{p=0},{p=1},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=1}},
    [5] = {{p=1},{p=0},{p=1},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=1}},
    [6] = {{p=1},{p=0},{p=1},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=1}},
    [7] = {{p=1},{p=0},{p=1},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=1}},
    [8] = {{p=1},{p=0},{p=1},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=1}},
    [9] = {{p=1},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=1}},
    [10] = {{p=1},{p=0},{p=1},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=1}},
    [11] = {{p=1},{p=0},{p=1},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=1}},
    [12] = {{p=1},{p=0},{p=1},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=1}},
    [13] = {{p=1},{p=0},{p=1},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=1}},
    [14] = {{p=1},{p=0},{p=1},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=1}},
    [15] = {{p=1},{p=0},{p=1},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=1}},
    [16] = {{p=1},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=1}},
    [17] = {{p=1},{p=1},{p=1},{p=1},{p=1},{p=1},{p=1},{p=1},{p=1},{p=0},{p=1},{p=1},{p=1},{p=1},{p=1},{p=1},{p=1}},
}

local mapId = 92001
local function printMap()
    local mapData = map[mapId]
    local hsize = #mapData[1]
    local realData = ""
    for h=1, hsize, 1 do
        for _, htable in ipairs(mapData) do
            local v = htable[h]
            realData = realData .." " .. v.p
        end
        realData = realData .. "\n"
    end
    print(realData)
end



local function loadMapDataPath(mapId)
    return map[mapId]
end

AstarCore.RegLoadMapFunc(loadMapDataPath)

local gridData = AstarCore.Create(mapId)

for i=10, 1, -1 do
    local randomx = math.random(1,17)
    local randomy = math.random(1,17)
    map[mapId][randomx][randomy].p = 1
    AstarCore.SetGripGripMark(gridData,randomx,randomy,1)
end

print("\n初始地图---------------")
printMap()

local startX = 2
local startY = 2
local randomx = math.random(1,17)
local randomy = math.random(1,17)

print("寻路起点为：",startX,startY)
print("寻路终点为：",randomx,randomy)

local ret , pathList = AstarCore.FindPath(gridData,startX,startY,randomx,randomy)
if ret > 0 then
    for _, point in pairs(pathList) do
        map[mapId][point.x][point.y].p = "*"
    end
end
print("\n寻路后地图---------------")
printMap()

print("hasBarrier:",AstarCore.HasBarrier(gridData, 2,2,8,8))

collectgarbage("collect")
AstarCore.UnloadGripMap(mapId)


