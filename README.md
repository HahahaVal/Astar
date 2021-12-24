# Astar
基于A*算法实现的寻路算法

  * 若 #define DISABLE_GREEDY 则禁用贪婪算法进行路径检查
  * 自定义Map数据，重载operator() 操作符，用于边界检查以及检查是否可走
  * 使用lua脚本调用提供以下接口
    * AstarCore.RegLoadMapFunc(loadMapDataFunc)
    * local gridData = AstarCore.Create(mapId)
    * AstarCore.SetGripGripMark(gridData,x,y,mark)
    * AstarCore.FindPath(gridData,fromX,fromY,toX,toY)
    * Aster.UnloadGripMap(mapId)