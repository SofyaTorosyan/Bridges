#pragma once

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
    Vertex(int, int);
    Vertex() { }
    int bid                      = 0;
    int bridge_end               = 0;
    int bridge_start             = 0;
    int bid_count                = 0;
    int  count_for_corner_inners = 0;
    bool is_on_bridge            = false;
    bool is_on_bridge_end        = false;
    bool is_on_bridge_start      = false;
    Type type                    = Type::corner;
    Direction prev_direction     = None;
    Direction bridge_direction   = None;
};


