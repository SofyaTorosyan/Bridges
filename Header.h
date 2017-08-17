#ifndef HEADER_H
#define HEADER_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include  <utility> // std::pair

using namespace std;

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

int radius = 0;

struct Vertex : Point
{
    Vertex(int, int);
    Vertex() { }
    Vertex* up   ();
    Vertex* left ();
    Vertex* down ();
    Vertex* right();
    int  bid  = 0;
    int bridge_start = 0;
    int bridge_end = 0;
    int  count_for_corner_inners = 0;
    Type type = Type::corner;
    bool is_on_bridge          = false;
    bool is_on_bridge_end      = false;
    bool is_on_bridge_start    = false;
    Direction prev_direction   = None;
    Direction bridge_direction = None;
};

template<class T>
class Matrix
{
public:
    Matrix ();
    ~Matrix();
    void C4V   (int, int);
    int  rows  ();
    int  cols  ();
    void print ();
    void Circle();
    Vertex*& operator()    (int, int);
    void enumerateBuildings( );

    class iterator
    {
    public:
        using vertex_pointer = T;
        using reference = T&;
        using pointer = T*;

        iterator() = default;

        iterator(bool b) : is_on_building(b) { }

        int x() { return x_; }

        int y() { return y_; }

        void set_iterator()
        {
            int r = m.rows_;
            int c = m.cols_;
            int i = 0;
            while (r != 0)
            {
                while (c != 0)
                {
                    if (**this == nullptr)
                        *this->right();
                    else
                        return;
                    c--;
                }
                *this->update(0, ++i);
                r--;
            }
        }

        iterator left(int i = 1)
        {
            (*this).x_ -= i;
            return *this;
        }

        iterator right(int i = 1)
        {
            (*this).x_ += i;
            return *this;
        }

        iterator up(int i = 1)
        {
            (*this).y_ -= i;
            return *this;
        }

        iterator down(int i = 1)
        {
            (*this).y_ += i;
            return *this;
        }

        iterator left_v(int i = 1)
        {
            iterator temp = *this;
            temp.x_ -= i;
            return temp;
        }

        iterator right_v(int i = 1)
        {
            iterator temp = *this;
            temp.x_ += i;
            return temp;
        }

        iterator up_v(int i = 1)
        {
            iterator temp = *this;
            temp.y_ -= i;
            return temp;
        }

        iterator down_v(int i = 1)
        {
            iterator temp = *this;
            temp.y_ += i;
            return temp;
        }

        bool operator!=(const iterator& r_it)
        {
            return (x_ != r_it.x_ || y_ != r_it.y_);
        }

        bool operator==(const iterator& r_it)
        {
            return (x_ == r_it.x_ && y_ == r_it.y_);
        }

        void  move_Opposite_To_Bridge_Direction()
        {
            switch ((**this)->bridge_direction)
            {
            case Right:
                while (!(**this)->is_on_bridge_start)
                    left();
               // (**this)->prev_direction = prev_start;
                break;
            case Left:
                while (!(**this)->is_on_bridge_start)
                    right();
               // (**this)->prev_direction = prev_start;
                break;
            case Up:
                while (!(**this)->is_on_bridge_start)
                    down();
              //  (**this)->prev_direction = prev_start;
                break;
            case Down:
                while (!(**this)->is_on_bridge_start)
                    up();
              //  (**this)->prev_direction = prev_start;
                break;
            }
        }

        void move_Towards_To_Bridge_Direction()
        {
            switch ((**this)->bridge_direction)
            {
            case Right:
                while (!(**this)->is_on_bridge_end)
                    right();
               // (**this)->prev_direction = prev_end;
                break;
            case Left:
                while (!(**this)->is_on_bridge_end)
                    left();
               // (**this)->prev_direction = prev_end;
                break;
            case Up:
                while (!(**this)->is_on_bridge_end)
                    up();
                //(**this)->prev_direction = prev_end;
                break;
            case Down:
                while (!(**this)->is_on_bridge_end)
                    down();
               // (**this)->prev_direction = prev_end;
            }
        }

       void move_Clockwise_From_Wall()
        {
           
            switch ((**this)->prev_direction)
            {
            case Right: right();  (**this)->prev_direction = Right; break;
            case Left:  left();   (**this)->prev_direction = Left;  break;
            case Down:  down();   (**this)->prev_direction = Down;  break;
            case Up:    up();     (**this)->prev_direction = Up;    break;
            }
        }

        void move_From_Wall()
        {
            if (!(**this)->is_on_bridge_start)
            {
                if ((**this)->is_on_bridge_end && ++(**this)->bridge_end == 2)
                {
                    (**this)->bridge_end = 0;
                    move_Opposite_To_Bridge_Direction();
                  
                }
                else
                    move_Clockwise_From_Wall();
            }
            else
            {
                if ((**this)->is_on_bridge_start && (++(**this)->bridge_start == 1))
                    move_Towards_To_Bridge_Direction();
                else
                {
                    (**this)->bridge_start = 0;
                    move_Clockwise_From_Wall();
                  
                }
            }
        }
    
        void move_From_Corner(iterator start, int& count)
        {
            if (!(**this)->is_on_bridge_start)
            {
                if ((**this)->is_on_bridge_end && ++(**this)->bridge_end == 2)
                {
                    (**this)->bridge_end = 0;
                    move_Opposite_To_Bridge_Direction();
                   
                }
                else
                {

                    if ((**this)->is_on_bridge_end && (**this)->prev_direction == None)
                    {
                       // (**this)->prev_direction = Up;
                        move_Clockwise_From_Corner(start, count);
                      // (* (*this).left_v())->prev_direction = None;

                    }
                    else 
                        move_Clockwise_From_Corner(start, count);
                    return;
                    
                }
            }
            else
            {
                if (++(**this)->bridge_start == 1)
                    move_Towards_To_Bridge_Direction();
                else
                {
                    (**this)->bridge_start = 0;
                    move_Clockwise_From_Corner(start, count);
                   
                }
            }
        
        }

        void move_Clockwise_From_Corner(iterator start, int& count)
        {
            if ((**this)->prev_direction != None)   //Still it haven't come to the beginning.
            {
                if ((*this) != start)
                    switch ((**this)->prev_direction)
                    {
                    case Right: down();   break;
                    case Left:  up();     break;
                    case Down:  left();   break;
                    case Up:    right();  break;
                    }
                else
                {
                    if (++count == 1)
                        switch ((**this)->prev_direction)
                        {
                        case Right: down();   break;
                        case Left:  up();     break;
                        case Down:  left();   break;
                        case Up:    right();  break;
                        }
                       // move_Clockwise_From_Corner(start, count);
                }
            }

            else
            {
                cout << "it is at the beginning" << endl;
                if ((*this) == start)
                    count++;
                if(count!=2)
                    right();
                  
            }
            return;
        }

        void move_Clockwise_From_Corner_Inner()
        {
            switch ((**this)->prev_direction)
            {
            case Right: if ((**this)->count_for_corner_inners == 0) { (**this)->count_for_corner_inners = 1;  up();    }  else { (**this)->count_for_corner_inners = 0; down();  } break;
            case Left:  if ((**this)->count_for_corner_inners == 0) { (**this)->count_for_corner_inners = 1;  down();  }  else { (**this)->count_for_corner_inners = 0; up();    } break;
            case Down:  if ((**this)->count_for_corner_inners == 0) { (**this)->count_for_corner_inners = 1;  right(); }  else { (**this)->count_for_corner_inners = 0; left();  } break;
            case Up:    if ((**this)->count_for_corner_inners == 0) { (**this)->count_for_corner_inners = 1;  left();  }  else { (**this)->count_for_corner_inners = 0; right(); } break;
            }
        }

        void move_Clockwise_From_Inside(int& count)
        {
            (**this)->bid = (*(*this).left_v())->bid;
            max_bid--;
            count = 2;         // Go out from while.
        }

        iterator operator=( iterator& r_it)
        {
            x_ = r_it.x_;
            y_ = r_it.y_;
            return *this;
        }

        reference operator* ()
        {
            return m_(x_, y_);
        }

        pointer operator->() { return &m_(x_, y_); }

        bool is_valid()
        {
            if (x_ > m_.col_ || y_ > m_.row_)
                return false;
            else
                return m_(x_, y_) != nullptr;
        }

        iterator update(int x, int y)
        {
            x_ = x;
            y_ = y;
            return *this;
        }

    private:
        int x_              = 0;
        int y_              = 0;
        Matrix<T>& m_       = m;
        bool is_on_building = false;
    };

private:
    using M = vector <vector<Vertex*>>;
    static int max_bid;
    int  rows_;
    int  cols_;
    M    m_;
};

char**          hashdot;
ifstream        file("My_City.txt", std::ifstream::in);
Matrix<Vertex*> m;
vector<pair<Vertex*, Direction>> corner_inners;

int Matrix<Vertex*>::max_bid = 0;

bool contains(Vertex* v)
{
    return (m((*v).x, (*v).y)) != nullptr;
}

int row_size()
{
    string line;
    if (!file)
        cout << "There is no file";
    file.seekg(0, ios::beg);
    getline(file, line);
    return line.size() + 1;
}

int col_size()
{
    int count = 0;
    string line;
    if (!file)
        cout << "There is no file";
    file.seekg(0, ios::beg);
    while (getline(file, line))
        ++count;
    file.clear();
    return count + 1;
}

void connect_2_building(pair<Matrix<Vertex*>::iterator,char>& connectible, int bid)
{
    int i     = 0;
    int count = 0;
    pair<Matrix<Vertex*>::iterator,char> connectible_start = connectible;
    
    if((**connectible.first).bid!=bid)
    while (count != 2)
    {
        switch ((*connectible.first)->type)
        {
        case wall:
            (**connectible.first).bid = bid;
            if (connectible.first == connectible_start.first)
                ++count;
                if(count!=2)
            connectible.first.move_Clockwise_From_Wall();
            break;
        case corner:
            (**connectible.first).bid = bid;
            connectible.first.move_Clockwise_From_Corner(connectible_start.first,count);
            break;
        case corner_inner:
            (**connectible.first).bid = bid;
            connectible.first.move_Clockwise_From_Corner_Inner();
            break;
        }
    }

    for (unsigned int i = 0; i < corner_inners.size(); i++)
        m(corner_inners.at(i).first->x, corner_inners.at(i).first->y)->prev_direction = corner_inners.at(i).second;

    if (radius != 1)
        switch (connectible.second)
        {
        case 'R':
            (**connectible.first).is_on_bridge_end = true;
            (**connectible.first).bridge_direction = Right;
           
            connectible.first.left();
            while (*connectible.first == nullptr)
            {
                auto v = new Vertex(connectible.first.x(), connectible.first.y());
                m(connectible.first.x(), connectible.first.y())=v;
                (*connectible.first)->is_on_bridge = true;
              
                m(connectible.first.x(), connectible.first.y())->prev_direction = Right;
                connectible.first.left();
            }
            (**connectible.first).is_on_bridge_start = true;
            (**connectible.first).bridge_direction = Right;
            break;

        case 'L':
            (**connectible.first).is_on_bridge_end = true;
            (**connectible.first).bridge_direction = Left;
            connectible.first.right();
            while (*connectible.first == nullptr)
            {
                auto v = new Vertex(connectible.first.x(), connectible.first.y());
                m(connectible.first.x(), connectible.first.y()) = v;
                (*connectible.first)->is_on_bridge = true;
             
                m(connectible.first.x(), connectible.first.y())->prev_direction = Left;
                connectible.first.right();
            }
            (**connectible.first).is_on_bridge_start = true;
            (**connectible.first).bridge_direction = Left;
            break;
           
        case 'U':
            (**connectible.first).is_on_bridge_end = true;
            (**connectible.first).bridge_direction = Up;
            connectible.first.down();
            while (*connectible.first == nullptr)
            {
                auto v = new Vertex(connectible.first.x(), connectible.first.y());
                m(connectible.first.x(), connectible.first.y()) = v;
                (*connectible.first)->is_on_bridge = true;
               
                m(connectible.first.x(), connectible.first.y())->prev_direction = Up;
                connectible.first.down();
            }
            (**connectible.first).is_on_bridge_start = true;
            (**connectible.first).bridge_direction = Up;
            break;

        case 'D':
            (**connectible.first).is_on_bridge_end = true;
            (**connectible.first).bridge_direction = Down;
            connectible.first.up();
            while (*connectible.first == nullptr)
            {
                auto v = new Vertex(connectible.first.x(), connectible.first.y());
                m(connectible.first.x(), connectible.first.y()) = v;
                (*connectible.first)->is_on_bridge = true;
              
                m(connectible.first.x(), connectible.first.y())->prev_direction = Down;
                connectible.first.up();
            }
            (**connectible.first).is_on_bridge_start = true;
            (**connectible.first).bridge_direction = Down;
            break;
        }
}

pair<Matrix<Vertex*>::iterator,char> find_Isolated_Building(pair<Matrix<Vertex*>::iterator,char> it, int bid)
{
    if ((**it.first).x == 0)
    {
        if (((**it.first).x + radius) < m.rows())
            if ((*it.first.right_v(radius)) != nullptr)
                if ((**it.first.right_v(radius)).bid != (**it.first).bid && (**it.first.right_v(radius)).bid != 0)
                {
                    it.first.right(radius);
                    (it.second) = 'R';
                    return it;
                }
    }

        if ((**it.first).x == m.rows() - 1)
        {
            if (((**it.first).x - radius) > 0)
                if ((*it.first.left_v(radius)) != nullptr)
                    if ((**it.first.left_v(radius)).bid != (**it.first).bid && (**it.first.left_v(radius)).bid != 0)
                    {
                        it.first.left(radius);
                        (it.second) = 'L';
                        return it;
                    }
        }

        else
        {
            if ((**it.first).x + radius < m.rows())
                if ((*it.first.right_v(radius)) != nullptr)
                    if ((**it.first.right_v(radius)).bid != (**it.first).bid && (**it.first.right_v(radius)).bid != 0)
                    {
                        it.first.right(radius);
                        (it.second) = 'R';
                        return it;
                    }

            if (((**it.first).x - radius) > 0)
                if ((*it.first.left_v(radius)) != nullptr)
                    if ((**it.first.left_v(radius)).bid != (**it.first).bid && (**it.first.left_v(radius)).bid != 0)
                    {
                        it.first.left(radius);
                        (it.second) = 'L';
                        return it;
                    }
        }

        if ((**it.first).y == 0)
        {
            if (((**it.first).y + radius) < m.cols())
                if ((*it.first.down_v(radius)) != nullptr)
                    if ((**it.first.down_v(radius)).bid != (**it.first).bid && (**it.first.down_v(radius)).bid != 0)
                    {
                        it.first.down(radius);
                        (it.second) = 'D';
                        return it;
                    }
        }

        if ((**it.first).y == m.cols() - 1)
        {
            if (((**it.first).y - radius) > 0)
                if ((*it.first.up_v(radius)) != nullptr)
                    if ((**it.first.up_v(radius)).bid != (**it.first).bid && (**it.first.up_v(radius)).bid != 0)
                    {
                        it.first.up(radius);
                        (it.second) = 'U';
                        return it;
                    }
        }

        else
        {
            if (((**it.first).y + radius) < m.cols())
                if ((*it.first.down_v(radius)) != nullptr)
                    if ((**it.first.down_v(radius)).bid != (**it.first).bid && (**it.first.down_v(radius)).bid != 0)
                    {
                        it.first.down(radius);
                        (it.second) = 'D';
                        return it;
                    }

            if (((**it.first).y - radius) > 0)
                if ((*it.first.up_v(radius)) != nullptr)
                    if ((**it.first.up_v(radius)).bid != (**it.first).bid && (**it.first.up_v(radius)).bid != 0)
                    {
                        it.first.up(radius);
                        (it.second) = 'U';
                        return it;
                    }
        }
        return pair<Matrix<Vertex*>::iterator, char>();
}

void find_new_building(Matrix<Vertex*>::iterator& it)

{
    int bid  = (**it).bid;
    int rows = (**it).x;
    int cols = (**it).y;
    int i    = 0;
    while (cols != m.cols()-1)
    {
        while (rows != m.rows()-1)
        {
            if ((*it.right_v()) != nullptr)
                if ((**it.right()).bid != bid && (**it).bid!=0)
                    return;
                else
                    rows++;
            else
            {
                it.right();
                rows++;
            }
        }
        cols++;
        it.update(0, ++i);
        rows= 0;
    }
}

Vertex::Vertex(int a, int b)
{
    x = a;
    y = b;
    bid = 0;
}

Vertex* Vertex::up()
{
    if ((*this).y != 0)
    {
        Vertex* vup = this;
        (*vup).x = (*this).x;
        (*vup).y = (*this).y - 1;
        if (contains(vup))
            return vup;
    }
    return nullptr;
}

Vertex* Vertex::left()
{
    if ((*this).x != 0)
    {
        Vertex* vleft = this;
        (*vleft).x = (*this).x - 1;
        (*vleft).y = (*this).y;
        if (contains(vleft))
            return vleft;
    }
    return nullptr;
}

Vertex* Vertex::down()
{
    if ((*this).y != m.cols() + 1)
    {
        Vertex* vdown = this;
        (*vdown).y = (*this).y + 1;
        (*vdown).x = (*this).x;
        if (contains(vdown))
            return vdown;
    }
    return  nullptr;
}

Vertex* Vertex::right()
{
    if ((*this).x != m.rows())
    {
        Vertex* vright = this;
        (*vright).x = (*this).x + 1;
        (*vright).y = (*this).y;
        if (contains(vright))
            return vright;
    }
    return  nullptr;
}

template<class T>
Matrix<T>::Matrix() : rows_(row_size()), cols_(col_size())
{
    m_.resize(rows_);
    for (auto& col : m_)
        col.resize(cols_);
}

template<class T>
Matrix<T>::~Matrix()
{
    for (int j = 0; j < m.cols_; j++)
        for (int i = 0; i < m.rows_; i++)
            delete m(i, j);
}

template<class T>
int Matrix<T>::rows()
{
    return rows_;
}

template<class T>
int Matrix<T>::cols()
{
    return cols_;
}

template<class T>
void Matrix<T>::print()
{
    cout << "City without bridges" << '\n';
    for (int j = 0; j < cols_; j++)
    {
        for (int i = 0; i < rows_; i++)
        {
            if (m(i,j)==nullptr)
                cout << " ";
            else
            {
                if ((*m(i, j)).is_on_bridge)
                        cout << '.';
                else
                    cout << 'o';
            }
        }
        cout << '\n';
    }
}

char left(int x, int y)
{
    if (x == 0)
        return '.';
    return hashdot[x - 1][y];
}

char up(int x, int y)
{
    if (y == 0)
        return '.';
    return hashdot[x][y - 1];
}

char upleft(int x, int y)
{
    if (y == 0 || x == 0)
        return '.';
    return hashdot[x - 1][y - 1];
}

template<class T>
void Matrix<T>::C4V(int x, int y)
{
    auto v = new Vertex[4];
    v[0] = { x,   y };
    v[1] = { x,   y + 1 };
    v[2] = { x + 1, y + 1 };
    v[3] = { x + 1, y };

    if (left(x, y) == '#')
    {
        v[0].type = wall;
        v[1].type = wall;
    }

    if (up(x, y) == '#')
    {
        if ((*m(x + 1, y)).type == wall)
            v[3].type = corner_inner;
        else
            v[3].type = wall;

        v[0].type = wall;
    }
    else
        if (m(x + 1, y) != nullptr)
            v[3].type = corner_inner;

    if (x != 0 && y != 0)
    {
        if (left(x, y) == '#' || up(x, y) == '#')
        {
            switch ((*m(x, y)).type)
            {
            case corner_inner:
            {
                if (upleft(x, y) == '#')
                    v[0].type = inside;
                else
                    v[0].type = corner_inner;
            }
            break;
            case corner:
                v[0].type = wall;
                break;
            case wall:
                v[0].type = corner_inner;
                break;
            }

            if (up(x, y) == '#')
                if ((*m(x + 1, y)).type == wall)
                    v[3].type = corner_inner;
                else
                    v[3].type = wall;

            if (left(x, y) == '#')
                v[1].type = wall;
        }

        else
            if (m(x, y) != nullptr)
                v[0].type = corner_inner;

        if (up(x, y) != '#')
            if (x != m.rows_ - 1 && m(x + 1, y) != nullptr)
                v[3].type = corner_inner;
    }

    m(x, y) = &v[0];
    m(x, y + 1) = &v[1];
    m(x + 1, y + 1) = &v[2];
    m(x + 1, y) = &v[3];
}

template<class T>
Vertex*& Matrix<T>:: operator() (int r, int c)
{
    return m_[r][c];
}

template<class T>
void Matrix<T>::enumerateBuildings()
{
    Matrix<Vertex*>::iterator it;
    Matrix<Vertex*>::iterator start = it;
    bool go_out = true;
    int r = m.rows_;
    int c = m.cols_;
    int i = 0;
    while (m.cols_ != 0)
    {
        while (m.rows_ != 0)
        {
            cout << endl;
            if ((*it) != nullptr)
                if ((*it)->bid == 0)    
                {
                    start = it;
                    max_bid++;
                    int count = 0;            // For knowing that it came to the beginning again.
                    while (count != 2)
                    {
                        (*it)->bid = max_bid;
                      
                        switch ((*it)->type)
                        {
                        case inside:
                            it.move_Clockwise_From_Inside(count);
                            break;
                        case wall:
                            it.move_Clockwise_From_Wall();
                            break;
                        case corner:
                            if (it != start)   //Still it haven't come to the beginning.
                                switch ((**it).prev_direction)
                                {
                                case Right:  it.down();  (**it).prev_direction = Down;   break;
                                case Left:   it.up();    (**it).prev_direction = Up;     break;
                                case Down:   it.left();  (**it).prev_direction = Left;   break;
                                case Up:     it.right(); (**it).prev_direction = Right;  break;
                                }                                                                                                        
                            else                //When it is at the beginning of building.(prev_direction=-1)
                            {
                                cout << "it is at the beginning" << endl;
                                (**it).prev_direction = None;
                                if (count == 0)
                                {
                                    it.right();
                                    (**it).prev_direction = Right;
                                }
                                count++;
                            }
                            break;
                        case corner_inner:
                            switch ((**it).prev_direction)
                            {
                            case Right:  corner_inners.push_back(make_pair((*it), Right));  it.up();    (**it).prev_direction  = Up;    break;
                            case Left:                                                      it.down();  (**it).prev_direction = Down;   break;
                            case Down:   corner_inners.push_back(make_pair((*it), Down));   it.right(); (**it).prev_direction  = Right; break;
                            case Up:                                                        it.left();  (**it).prev_direction = Left;   break;
                            }
                            break;
                        }
                    }

                    for (unsigned int i = 0; i < corner_inners.size(); i++)
                        m(corner_inners.at(i).first->x, corner_inners.at(i).first->y)->prev_direction=corner_inners.at(i).second;
                }
            m.rows_--;
            if (m.rows_ > 1)
                it.right();
        }
        m.rows_ = r;
        m.cols_--;
        if (m.cols_ != 0)
            it = it.update(0, ++i);
    }
    //Recover m.rows_ and m.cols_ values.
    m.rows_ = r;
    m.cols_ = c;
}

Direction prev_end   = None;
Direction prev_start = None;

int count_of_bridges = 0;
int non_connected_buildings=0;

template<class T>
void Matrix<T>::Circle()
{
    int count;
    int r = 0;
    
   
    Matrix<Vertex*>::iterator             circle_start;
    pair<Matrix<Vertex*>::iterator, char> it;
    pair<Matrix<Vertex*>::iterator, char> connectible;
    vector< Matrix<Vertex*>::iterator > bridge_starts;
    vector< Matrix<Vertex*>::iterator > bridge_ends;
    it.first.set_iterator();
    circle_start      = it.first;
    connectible.first = circle_start;
    int l = min(m.cols(),m.rows()) - 1;
   
     non_connected_buildings = max_bid;
    while (it.first.x() != rows_ - 1 && it.first.y() != cols_ - 1 && max_bid!=1)
    {

        while (radius != l && max_bid != 1)
        {
            it.first = circle_start;
            radius++;
            count = 0;
            while (count != 2)
            {
                connectible = find_Isolated_Building(it,(**it.first).bid);
                if (connectible.first != pair<iterator, char>().first && connectible.second != pair<iterator, char>().second && (**it.first).bid!=(**connectible.first).bid && (**it.first).bid!=0)
                {
                    bridge_starts.push_back(it.first);
                    bridge_ends.push_back(connectible.first);
                    connect_2_building(connectible, (**circle_start).bid);
                    count    = 0;
                    radius   = 1;
                    for (unsigned int i = 0; i < corner_inners.size(); i++)
                        m(corner_inners.at(i).first->x, corner_inners.at(i).first->y)->count_for_corner_inners = 0;
                    it.first = circle_start;
                    max_bid--;
                    count_of_bridges++;
                    non_connected_buildings--;

                    for (unsigned int i = 0; i < bridge_starts.size(); i++)
                        (**bridge_starts[i]).bridge_start = 0;

                    for (unsigned int i = 0; i < bridge_ends.size(); i++)
                        (**bridge_ends[i]).bridge_end = 0;
                }
                      
                switch ((*it.first)->type)
                {
                case wall:
                    it.first.move_From_Wall();
                    break;
                case corner:
                    it.first.move_From_Corner(circle_start, count);
                    break;
                case corner_inner:
                    it.first.move_Clockwise_From_Corner_Inner();
                    break;
                }
            }

            for (unsigned int i = 0; i < corner_inners.size(); i++)
                m(corner_inners.at(i).first->x, corner_inners.at(i).first->y)->prev_direction = corner_inners.at(i).second;
        }

        if (max_bid != 1)
        {
            find_new_building(it.first);
            max_bid--;
            circle_start = it.first;
            radius = 0;
        }
    }
}

#endif