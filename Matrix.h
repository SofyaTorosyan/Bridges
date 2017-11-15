#pragma once


#include "Timer.h"
#include "HashDot.h"
#include "Vertex.cpp"


template<class T>
class Matrix
{
public:
    HashDot  hash_dot;
    Matrix();
    ~Matrix();
    void C4V(int, int);
    int  rows();
    int  cols();
    void print();
    void Circle();
    Vertex*& operator() (int, int);
    void Create_Vertexes();
    void Write_To_File(const string);
    void enumerateBuildings();
    int Disconnected();

    class iterator
    {
    public:
        using vertex_pointer = T;
        using reference = T&;
        using pointer = T*;
        iterator() = default;
        iterator(bool); 
        int x();
        int y();
        void set_iterator();
        iterator move_left(int = 1);
        iterator move_right(int = 1);   
        iterator move_up(int = 1);
        iterator move_down(int = 1);
        iterator left_v(int = 1);
        iterator right_v(int = 1);
        iterator up_v(int = 1);
        iterator down_v(int = 1);
        bool operator!=(const iterator&);
        bool operator==(const iterator&);
        void  move_Opposite_To_Bridge_Direction(Direction);
        void move_Towards_To_Bridge_Direction(Direction);
        void move_Clockwise_From_Wall();  
        void move_From_Wall();
        void move_From_Corner(iterator , int& ); 
        void move_Clockwise_From_Corner(iterator, int&);
        void move_Clockwise_From_Corner_Inner();
        void move_Clockwise_From_Inside(int&);
        iterator& operator=(iterator&); 
        reference operator* ();
        pointer operator->();
        bool is_valid();
        iterator update(int, int);
    private:
        int x_ = 0;
        int y_ = 0;
        Matrix<T>& m_ = m;
        bool is_on_building_ = false;
        Direction prevmove_ = None;
    };
    bool is_symmetric_corner_inner(iterator);
private:
    using M = vector <vector<Vertex*>>;
    static int max_bid;
    int  rows_;
    int  cols_;
    M    m_;
};

