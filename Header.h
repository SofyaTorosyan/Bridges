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

enum Move
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
    Vertex* up();
    Vertex* left();
    Vertex* down();
    Vertex* right();
    Type type = Type::corner;
    int  bid = 0;
    bool is_on_bridge       = false;
    bool is_on_bridge_start = false;
    bool is_on_bridge_end   = false;
    Move prevmove = None;
    Move bridge_direction = None;
};

template<class T>
class Matrix
{
public:
    Matrix();
    ~Matrix();
    int  rows ();
    int  cols ();
    void print();
    void C4V(int, int);
    Vertex*& operator()(int, int);
    void Circle();
    void enumerateBuildings( );

    class iterator
    {
    public:
        using vertex_pointer = T;
        using reference = T&;
        using pointer = T*;
     
        iterator() = default;
    
        iterator(bool b) : is_on_building(b) { }

        int x()   { return x_; }
        
        int y()   { return y_; }
       
        void set_iterator() 
        {
            int r = m.rows_;
            int c = m.cols_;
            int i = 0;
            while (r != 0)
            {
                while (c != 0)
                {
                    if (this == nullptr)
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
            if (**this != nullptr)
            (**this)->prevmove = Left;
            return *this;
        }

        iterator right(int i = 1)
        {
            (*this).x_ += i;
            if(**this!=nullptr)
            (**this)->prevmove = Right;
            return *this;
        }

        iterator up(int i = 1)
        {
            (*this).y_ -= i;
            if (**this != nullptr)
            (**this)->prevmove = Up;
            return *this;
        }

        iterator down(int i = 1)
        {
            (*this).y_ += i;
            if (**this != nullptr)
            (**this)->prevmove = Down;
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

        void move_Clockwise_From_Wall() 
        {
            switch ((**this)->prevmove)
            {
            case Right: right(); break;
            case Left:  left();  break;
            case Down:  down();  break;
            case Up:    up();    break;
            }
        }

        void move_Clockwise_From_Corner(iterator start, int& count)
        {
            if ((**this)->prevmove != None)   //Still it haven't come to the beginning.
            {
                if ((*this) != start)
                    switch ((**this)->prevmove)
                    {
                    case Right:  down();  break;
                    case Left:   up();    break;
                    case Down:   left();  break;
                    case Up:     right(); break;
                    }
                else
                {
                    count++;
                    (**this)->prevmove = None;

                }
            }
            else                //When it is at the beginning of building.(prevmove=-1)
            {
                cout << "it is at the beginning" << endl;
                (**this)->prevmove = None;
                if (count == 0)
                    right();
                count++;
            }
            return;
        }

        void move_Clockwise_From_Corner_Inner()
        {
            switch ((**this)->prevmove)
            {
            case Right: up();    break;
            case Left:  down();  break;
            case Down:  right(); break;
            case Up:    left();  break;
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
vector<pair<Vertex*, Move>> corner_inners;

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

void connect_2_building(pair<Matrix<Vertex*>::iterator,char>& it, int bid, int radius)
{
    pair<Matrix<Vertex*>::iterator,char> start = it;
    int count = 0;

    if((**it.first).bid!=bid)
    while (count != 2)
    {
        switch ((*it.first)->type)
        {
        case wall:
            it.first.move_Clockwise_From_Wall();
            (**it.first).bid = bid;
            break;
        case corner:
            it.first.move_Clockwise_From_Corner(start.first,count);
            (**it.first).bid = bid;
            break;
        case corner_inner:
            it.first.move_Clockwise_From_Corner_Inner();
            (**it.first).bid = bid;
            break;
        }
    }

    for (unsigned int i = 0; i < corner_inners.size(); i++)
        m(corner_inners.at(i).first->x, corner_inners.at(i).first->y)->prevmove = corner_inners.at(i).second;

    if (radius != 1)
        switch (it.second)
        {
        case 'R':
            (**it.first).is_on_bridge_end = true;
            it.first.left();
            while (*it.first == nullptr)
            {
                auto v = new Vertex(it.first.x(), it.first.y());
                m(it.first.x(),it.first.y())=v;
                (*it.first)->is_on_bridge = true;
                m(it.first.x(), it.first.y())->prevmove = Left;
                it.first.left();
            }
            (**it.first).is_on_bridge_start = true;
            (**it.first).bridge_direction = Right;
            break;

        case 'L':
            (**it.first).is_on_bridge_end = true;
            it.first.right();
            while (*it.first == nullptr)
            {
                auto v = new Vertex(it.first.x(), it.first.y());
                m(it.first.x(), it.first.y()) = v;
                (*it.first)->is_on_bridge = true;
                m(it.first.x(), it.first.y())->prevmove = Right;  //(**it.first).prevmove = Right;
                it.first.right();
            }
            (**it.first).is_on_bridge_start = true;
            (**it.first).bridge_direction = Left;
            break;
           
        case 'U':
            (**it.first).is_on_bridge_end = true;
            it.first.down();
            while (*it.first == nullptr)
            {
                auto v = new Vertex(it.first.x(), it.first.y());
                m(it.first.x(), it.first.y()) = v;
                (*it.first)->is_on_bridge = true;
                m(it.first.x(), it.first.y())->prevmove = Down;
                it.first.down();
            }
            (**it.first).is_on_bridge_start = true;
            (**it.first).bridge_direction = Up;
            break;

        case 'D':
            (**it.first).is_on_bridge_end = true;
            it.first.up();
            while (*it.first == nullptr)
            {
                auto v = new Vertex(it.first.x(), it.first.y());
                m(it.first.x(), it.first.y()) = v;
                (*it.first)->is_on_bridge = true;
                m(it.first.x(), it.first.y())->prevmove = Up;
                it.first.up();
            }
            (**it.first).is_on_bridge_start = true;
            (**it.first).bridge_direction = Down;
            break;
        }
}

bool is_repeated(vector<pair< Matrix<Vertex*>::iterator, char>>& vertexes, Matrix<Vertex*>::iterator it)
{
    unsigned int i = 0;
    for (; i < vertexes.size(); i++)
        if ((it.x() == vertexes[i].first.x() || it.y() == vertexes[i].first.y()) && (**it).bid == (**vertexes[i].first).bid)
            return true;
    return false;
}

void find_Isolated_Building(Matrix<Vertex*>::iterator& it, int radius, int bid, vector<pair<Matrix<Vertex*>::iterator, char>>& v)
{
    if ((**it).x == 0)
    {
        if (((**it).x + radius) < m.rows())
            if ((*it.right_v(radius)) != nullptr)
                if ((**it.right_v(radius)).bid != (**it).bid)
                {
                    if (!is_repeated(v, it.right_v(radius)))
                        v.push_back(make_pair(it.right_v(radius), 'R'));  // vector of pair for not changing prevmoves and having directions-R D L U
                }
    }

        if ((**it).x == m.rows() - 1)
        {
            if (((**it).x - radius) > 0)
                if ((*it.left_v(radius)) != nullptr)
                    if ((**it.left_v(radius)).bid != (**it).bid)
                    {
                        if (!is_repeated(v, it.left_v(radius)))
                            v.push_back(make_pair(it.left_v(radius), 'L'));
                    }
        }

        else
        {
            if ((**it).x + radius < m.rows())
                if ((*it.right_v(radius)) != nullptr)
                    if ((**it.right_v(radius)).bid != (**it).bid)
                    {
                        if (!is_repeated(v, it.right_v(radius)))
                            v.push_back(make_pair(it.right_v(radius), 'R'));
                    }


            if (((**it).x - radius) > 0)
                if ((*it.left_v(radius)) != nullptr)
                    if ((**it.left_v(radius)).bid != (**it).bid)
                    {
                        if (!is_repeated(v, it.left_v(radius)))
                            v.push_back(make_pair(it.left_v(radius), 'L'));
                    }
        }

        if ((**it).y == 0)
        {
            if (((**it).y + radius) < m.cols())
                if ((*it.down_v(radius)) != nullptr)
                    if ((**it.down_v(radius)).bid != (**it).bid)
                    {
                        if (!is_repeated(v, it.down_v(radius)))
                            v.push_back(make_pair(it.down_v(radius), 'D'));
                    }
        }

        if ((**it).y == m.cols() - 1)
        {
            if (((**it).y - radius) > 0)
                if ((*it.up_v(radius)) != nullptr)
                    if ((**it.up_v(radius)).bid != (**it).bid)
                    {
                        if (!is_repeated(v, it.up_v(radius)))
                            v.push_back(make_pair(it.up_v(radius), 'U'));
                    }
        }

        else
        {
            if (((**it).y + radius) < m.cols())
                if ((*it.down_v(radius)) != nullptr)
                    if ((**it.down_v(radius)).bid != (**it).bid)
                    {
                        if (!is_repeated(v, it.down_v(radius)))
                            v.push_back(make_pair(it.down_v(radius), 'D'));
                    }

            if (((**it).y - radius) > 0)
                if ((*it.up_v(radius)) != nullptr)
                    if ((**it.up_v(radius)).bid != (**it).bid)
                    {
                        if (!is_repeated(v, it.up_v(radius)))
                            v.push_back(make_pair(it.up_v(radius), 'U'));
                    }
        }
    }

void find_new_building(Matrix<Vertex*>::iterator& it)
{
    int bid = ++(**it).bid;
    int r = m.rows();
    int c = m.cols();
    int i = 0;
    while (c != 1)
    {
        while (r != 1)
        {
            if ((*it.right_v()) != nullptr)
                if ((**it.right()).bid == bid)
                    return;
                else
            r--;
            else
            it.right();
        }
        c--;
        it.update(0, ++i);
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
                            switch ((**it).prevmove)
                            {
                            case Right: it.right(); break;
                            case Left:  it.left();  break;
                            case Down:  it.down();  break;
                            case Up:    it.up();    break;
                            }
                            break;
                        case corner:
                            if (it != start)   //Still it haven't come to the beginning.
                                switch ((**it).prevmove)
                                {
                                case Right:  it.down();    break;
                                case Left:   it.up();      break;
                                case Down:   it.left();    break;
                                case Up:     it.right();   break;
                                }                                                                                                        
                            else                //When it is at the beginning of building.(prevmove=-1)
                            {
                                cout << "it is at the beginning" << endl;
                                (**it).prevmove = None;
                                if (count == 0)
                                    it.right();
                                count++;
                            }
                            break;
                        case corner_inner:
                            switch ((**it).prevmove)
                            {
                            case Right:  corner_inners.push_back(make_pair((*it), Right));  it.up();    break;
                            case Left:                                                      it.down();  break;
                            case Down:   corner_inners.push_back(make_pair((*it), Down));   it.right(); break;
                            case Up:                                                        it.left();  break;
                            }                        
                            break;
                        }
                    }

                    for (unsigned int i = 0; i < corner_inners.size(); i++)
                        m(corner_inners.at(i).first->x, corner_inners.at(i).first->y)->prevmove=corner_inners.at(i).second;
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

template<class T>
void Matrix<T>::Circle()
{
    int count;
    int radius = 0;
    Matrix<Vertex*>::iterator it;
    Matrix<Vertex*>::iterator start;
    int r = 0;

    it.set_iterator();
    start = it;

    while (max_bid != 1)
    {
        radius++;
        count = 0;
        vertexes.clear();
        while (count != 2)
        {
            find_Isolated_Building(it, radius, (**it).bid, vertexes);
            switch ((*it)->type)
            {
            case wall:
                it.move_Clockwise_From_Wall();
                break;
            case corner:
                it.move_Clockwise_From_Corner(start, count);
                break;
            case corner_inner:
                it.move_Clockwise_From_Corner_Inner();
                break;
            }
        }

        for (unsigned int i = 0; i < corner_inners.size(); i++)
            m(corner_inners.at(i).first->x, corner_inners.at(i).first->y)->prevmove = corner_inners.at(i).second;

        if (!vertexes.empty())
            for (unsigned int i = 0; i < vertexes.size(); i++)
                connect_2_building(vertexes.at(i), (**it).bid, radius);
        max_bid--;
    }
}

#endif