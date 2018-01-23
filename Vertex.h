#ifndef VERTEX_H
#define VERTEX_H
struct Point
{
    int x;
    int y;
};

enum Type
{
    corner,
    corner_inner,
    wall,
    inside
};

enum Direction
{
    Up,
    Down,
    Left,
    Right,
    None
};

struct Vertex : Point
{
    Vertex(int a, int b)
    {
        x = a;
        y = b;
        bid = 0;
    }

    Vertex() { }
    int bid = 0;
    int bridge_end = 0;
    int bridge_start = 0;
    int bid_count = 0;
    int  count_for_corner_inners = 0;
    bool is_on_bridge = false;
    bool is_on_bridge_end = false;
    bool is_on_bridge_start = false;
    Type type = Type::corner;
    Direction prev_direction = None;
    Direction bridge_direction = None;
};
namespace
{
    int radius = 0;
    Direction prev_end = None;
    Direction prev_start = None;
    int count_of_bridges = 0;
    int bridge_length = 0;
    int non_connected_buildings = 0;
}

#endif