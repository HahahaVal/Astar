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

print("初始地图---------------")
printMap()

local function loadMapDataPath(mapId)
    return map[mapId]
end

AstarCore.RegLoadMapFunc(loadMapDataPath)

local gridData = AstarCore.Create(mapId)
if gridData then
    AstarCore.SetGripGripMark(gridData,2,3,1)
    local ret , pathList = AstarCore.FindPath(gridData,2,2,5,5)
    if ret > 0 then
        for _, point in pairs(pathList) do
            map[mapId][point.x][point.y].p = "*"
        end
    end
end
collectgarbage("collect")
AstarCore.UnloadGripMap(mapId)

print("寻路后地图---------------")
printMap()
