local AstarCore = require "AstarCore"

local map = {}

map[92001] = {
    [1] = {{p=1},{p=1},{p=1},{p=1},{p=1},{p=1},{p=1},{p=1},{p=1},{p=0},{p=1},{p=1},{p=1},{p=1},{p=1},{p=1},{p=1}},
    [2] = {{p=1},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=1}},
    [3] = {{p=1},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=1}},
    [4] = {{p=1},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=1}},
    [5] = {{p=1},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=1}},
    [6] = {{p=1},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=1}},
    [7] = {{p=1},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=1}},
    [8] = {{p=1},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=1}},
    [9] = {{p=1},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=1}},
    [10] = {{p=1},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=1}},
    [11] = {{p=1},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=1}},
    [12] = {{p=1},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=1}},
    [13] = {{p=1},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=1}},
    [14] = {{p=1},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=1}},
    [15] = {{p=1},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=1}},
    [16] = {{p=1},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=0},{p=1}},
    [17] = {{p=1},{p=1},{p=1},{p=1},{p=1},{p=1},{p=1},{p=1},{p=1},{p=0},{p=1},{p=1},{p=1},{p=1},{p=1},{p=1},{p=1}},
}

local mapId = 92001
local function printMap()
    local ret = ""
    for _, wtable in ipairs(map[mapId]) do
        for _, v in pairs(wtable) do
            ret = ret .." " .. v.p
        end
        ret = ret .. "\n"
    end
    print(ret)
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

print("初始地图---------------")
printMap()

local randomx = math.random(1,17)
local randomy = math.random(1,17)
print("寻路终点为：",randomx,randomy)

local ret , pathList = AstarCore.FindPath(gridData,2,2,randomx,randomy)
if ret > 0 then
    for _, point in pairs(pathList) do
        map[mapId][point.x][point.y].p = "*"
    end
end
print("寻路后地图---------------")
printMap()

collectgarbage("collect")
AstarCore.UnloadGripMap(mapId)


