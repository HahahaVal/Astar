#ifndef __Astar_H__
#define __Astar_H__

#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>
#include <stdio.h>

// #define DISABLE_GREEDY

typedef enum
{
    UNKNOWN = 0,    //初始
    OPEN = 1,       //待处理
    CLOSE = 2,      //已处理
}Flag;

enum Result
{
	NO_PATH,
	FOUND_PATH,
	NEED_MORE_STEPS,
	EMPTY_PATH
};

class Point
{
public:
    int x, y;

    Point(int _x=-1, int _y=-1) : x(_x),y(_y) {}

    inline bool operator==(const Point point) const
    {
        return x == point.x && y == point.y;
    }
    inline bool operator!=(const Point point) const
    {
        return x != point.x || y != point.y;
    }
    inline bool operator<(const Point& p) const
	{
		return y < p.y || (y == p.y && x < p.x);
	}

    inline bool is_valid() const { return (x != -1)&&(y != -1); }
};


class Node
{
public:
    Node(const Point& p) : g(0),f(0),pos(p),parent(NULL),flag(UNKNOWN) {}

    int g;  //当前点到起点的真实距离,前一个节点的g值加上两节点之间的距离
    int f;  //路径评分  f = g+h , h = 当前节点到终点的“估算距离
    const Point pos;
    const Node *parent;

    inline void set_open() { flag = OPEN; }
    inline void set_close() { flag = CLOSE; }
	inline unsigned is_open() const { return flag == OPEN; }
	inline unsigned is_closed() const { return flag == CLOSE; }
    inline void clear_state() { f = 0; g = 0; parent = NULL; flag = UNKNOWN; }

    ~Node(){};

private:
    Flag flag;
};

inline unsigned manhattan(const Node *node1, const Node *node2)
{
    return abs(int(node1->pos.x - node2->pos.x)) + abs(int(node1->pos.y - node2->pos.y));
}

inline unsigned euclidean(const Node *node1, const Node *node2)
{
    float fx = float(int(node1->pos.x - node2->pos.x));
    float fy = float(int(node1->pos.y - node2->pos.y));
    return unsigned(int(sqrtf(fx*fx + fy*fy)));
}


typedef std::vector<Node*> NodeVector;

class OpenList
{
public:
    inline void push(Node *node)
    {  
        //push_heap的功能是往‘堆’中插入一个数据。
        //默认前提是这个区间[first,last)已经满足堆结构，并且要插入的数据已经插入到堆的最后即区间的最后一个位置
        nodes.push_back(node);
        std::push_heap(nodes.begin(), nodes.end(), _compare);
    }
    inline Node *pop()
	{   
        //pop_head的功能是将堆顶的元素与最后一个元素交换，再执行调整程序，将除了区间最后一个元素（此时已经交换）的所有元素重新恢复堆结构。
        //当调用pop_head后再将区间的最后一个元素pop掉。
		std::pop_heap(nodes.begin(), nodes.end(), _compare);
		Node *node = nodes.back();
		nodes.pop_back();
		return node;
	}
    inline bool empty() const
	{
		return nodes.empty();
	}
    inline void clear()
	{
		nodes.clear();
	}
    inline void fix_heap()
	{
		std::make_heap(nodes.begin(), nodes.end(), _compare);
	}

private:
    static inline bool _compare(const Node *node1, const Node *node2)
	{
		return node1->f > node2->f;
	}
    NodeVector nodes;
};


typedef std::vector<Point> PathVector;
typedef std::map<Point, Node> NodeGrid;

template <typename GRID> class Searcher
{
public:
    Searcher(const GRID& g):grid(g){}

    Result find_path_init(Point start, Point end);
    Result find_path_step();
    bool find_path_finish(PathVector& path);
    
    bool find_path(PathVector& path, Point start, Point end);

private:
    const GRID& grid;              //实际格子数据，需要越界判断
    Node *start_node;
    Node *end_node;
    OpenList open_list;            //待处理的节点
    NodeGrid nodegrid;

    Node *get_node(const Point& pos);

    void confirm_follow(const Node *node);
    int find_neighbors(const Node *node, Point *wptr);

#ifndef DISABLE_GREEDY
	bool find_path_greedy(Node *node);
#endif

};



static const Point npos = Point();

template <typename GRID> inline Node *Searcher<GRID>::get_node(const Point& pos)
{
	return &nodegrid.insert(std::make_pair(pos, Node(pos))).first->second;
}

template <typename GRID> Result Searcher<GRID>::find_path_init(Point start, Point end)
{
    for(NodeGrid::iterator it = nodegrid.begin(); it != nodegrid.end(); ++it)
    {
        it->second.clear_state();
    }
    open_list.clear();

    start_node = NULL;
    end_node = NULL;

    if (start == end)
    {
        return grid(end.x, end.y) ? EMPTY_PATH : NO_PATH;
    }

    if(!grid(start.x, start.y) || !grid(end.x, end.y))
	{
        return NO_PATH;
    }

    start_node = get_node(start);
    end_node = get_node(end);

#ifndef DISABLE_GREEDY
	if(find_path_greedy(start_node))
		return FOUND_PATH;
#endif

    open_list.push(start_node);

    return NEED_MORE_STEPS;
}

template <typename GRID> bool Searcher<GRID>::find_path(PathVector& path, Point start, Point end)
{
    Result res = find_path_init(start, end);

    if(res == EMPTY_PATH)
		return true;
    while(true)
	{
		switch(res)
		{
			case NEED_MORE_STEPS:
				res = find_path_step();
				break;

			case FOUND_PATH:
				return find_path_finish(path);

			case NO_PATH:
			default:
				return false;
		}
	}
}

template <typename GRID> Result Searcher<GRID>::find_path_step()
{
	do
	{
		if(open_list.empty())
			return NO_PATH;
		Node *node = open_list.pop();
		node->set_close();
		if(node == end_node)
			return FOUND_PATH;
		confirm_follow(node);
	}
	while(1);
	return NEED_MORE_STEPS;
}


template <typename GRID> void Searcher<GRID>::confirm_follow(const Node *node)
{
    Point buff[4];
    int num = find_neighbors(node, &buff[0]);
    for(int i = num-1; i >= 0; --i)
	{
        Point neighbor_point = buff[i];
        Node *neighbor_node = get_node(neighbor_point);
        if (!neighbor_node->is_closed())
        {
            int dg = euclidean(neighbor_node, node);
            int newg = node->g + dg;

            if(!neighbor_node->is_open() || newg < neighbor_node->g)
            {
                neighbor_node->g = newg;
                neighbor_node->f = neighbor_node->g + manhattan(neighbor_node, end_node);
                neighbor_node->parent = node;
            
                if (!neighbor_node->is_open())
                {
                    open_list.push(neighbor_node);
                    neighbor_node->set_open();
                }
                else
                {
                    open_list.fix_heap();
                }
            }
        }
    }
}


template <typename GRID> int Searcher<GRID>::find_neighbors(const Node *node, Point *wptr)
{
    Point *w = wptr;
    int x = node->pos.x;
    int y = node->pos.y;
    //四个方向
    if(grid(x+1,y))
    {
        *w++ = Point(x+1, y);
    }
    if(grid(x-1,y))
    {
        *w++ = Point(x-1, y);
    }
    if(grid(x,y+1))
    {
        *w++ = Point(x, y+1);
    }
    if(grid(x,y-1))
    {
        *w++ = Point(x, y-1);
    }

    return int(w - wptr);
}

template<typename GRID> bool Searcher<GRID>::find_path_finish(PathVector& path)
{
    if (!end_node) 
        return false;

    int offset = path.size();

    const Node *next = end_node;
    if(!next->parent)
        return false;

    do
    {
        path.push_back(next->pos);
        
        if (next->parent)
        {
            next = next->parent;
        }

        if (next == start_node)
        {
            break;
        }
    }while (next->parent);

	std::reverse(path.begin() + offset, path.end());
	return true;
}

#ifndef DISABLE_GREEDY
//点与终点是否有障碍
template<typename GRID> bool Searcher<GRID>::find_path_greedy(Node *node)
{
    Point mid_pos = npos;
    int x = node->pos.x;
    int y = node->pos.y;
    int ex = end_node->pos.x;
    int ey = end_node->pos.y;

    if ( x==ex && y==ey )
    {
        return false;
    }

    int dx = int(ex-x);
    int dy = int(ey-y);
    const int adx = abs(dx);
	const int ady = abs(dy);
    //获取方向向量
	dx /= std::max(adx, 1);
	dy /= std::max(ady, 1);

    //如果是对角斜边，若当前点有效并且上或右的点有效，则往斜边走一格子继续判断
    if(x != ex && y != ey)
	{
        const int minlen = (int)std::min(adx, ady);
        const int tx = x + dx * minlen;

        for( ; x != tx; )
		{
			if(grid(x, y) && (grid(x+dx, y) || grid(x, y+dy)))
			{
				x += dx;
				y += dy;
			}
			else
				return false;
		}
		if(!grid(x, y))
			return false;

		mid_pos = Point(x, y);
    }

    //此时至少有一个轴是对齐的
    if(!(x==ex && y==ey))
    {
        while(x != ex)
            if(!grid(x += dx, y))
                return false;

        while(y != ey)
			if(!grid(x, y += dy))
				return false;
    }

	if(mid_pos.is_valid())
	{
        Node *mid = get_node(mid_pos);
		mid->parent = node;
		if(mid != end_node)
			end_node->parent = mid;
	}
	else
    {
        end_node->parent = node;
    }
		
	return true;
}
#endif

#endif