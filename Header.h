#ifndef HEADER_H
#define HEADER_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <windows.h>

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
    bool is_on_bridge = false;
};

template<class T>
class Matrix
{
public:
    Matrix();
    ~Matrix();
    int  rows();
    int  cols();
    void print();
    void C4V(int, int);
    Vertex*& operator()(int, int);
    void Circle();
    void enumerateBuildings();

    class iterator
    {
    public:
        Move prevmove = None;
        using vertex_pointer = T;
        using reference = T&;
        using pointer = T*;

        iterator() = default;
        iterator(bool b) : is_on_building(b) { }

        void set_iterator() // TODO: Make iterator find a non-connected building
        {
            int x, y;
            cin >> x >> y;
            x_ = x;
            y_ = y;
        }

        iterator left(int i = 1)
        {
            (*this).x_ -= i;
            if (i != 1)
                cout << x_ << "  " << y_ << endl;
            prevmove = Left;
            return *this;
        }

        iterator right(int i = 1)
        {
            (*this).x_ += i;
            if (i != 1)
                cout << x_ << "  " << y_ << endl;
            prevmove = Right;
            return *this;
        }

        iterator up(int i = 1)
        {
            (*this).y_ -= i;
            if (i != 1)
                cout << x_ << "  " << y_ << endl;
            prevmove = Up;
            return *this;
        }

        iterator down(int i = 1)
        {
            if (i != 1)
                cout << x_ << "  " << y_ << endl;
            (*this).y_ += i;
            prevmove = Down;
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

        void move_Clockwise_From_Wall() 
        {
            switch (prevmove)
            {
            case Right: right(); break;
            case Left:  left();  break;
            case Down:  down();  break;
            case Up:    up();    break;
            }
        }

        void move_Clockwise_From_Corner(iterator start, int& count)
        {
            if (*this != start)   //Still it haven't come to the beginning.
                switch (prevmove)
                {
                case Right: down();  break;
                case Left:  up();    break;
                case Down:  left();  break;
                case Up:    right(); break;
                }
            else                //When it is at the beginning of building.(prevmove=-1)
            {
                cout << "it is at the beginning" << endl;
                if (count == 0)
                    (*this).right();
                count++;
            }
        }

        void move_Clockwise_From_Corner_Inner()
        {
            switch (prevmove)
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

        bool operator!=(const iterator& r_it)
        {
            return (x_ != r_it.x_ || y_ != r_it.y_);
        }

        iterator operator=(const iterator& r_it)
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

        // TODO:  vector find_isolated_building(iterator, int bid, std::vector<iterator>& v) // TODO: Make this free function
        iterator find_isolated_building(int radius, int bid) // TODO: Make this free function
        {
            std::vector<iterator> v;
            /*
            if (x_ == 0)
            if ((*this).right(radius) != nullptr && (*(*this).right(radius))->bid != bid)
            v.push_back((*this).right(radius));
            if (x_ = m.rows_ - 1)
            if ((*this).left(radius) != nullptr && (*(*this).left(radius))->bid != bid)
            v.push_back((*this).right(radius));
            else
            if ((*this).right(radius) != nullptr && (*(*this).right(radius))->bid != bid)
            return  (*this).right(radius);
            if ((*this).left(radius) != nullptr && (*(*this).left(radius))->bid != bid)
            v.push_back((*this).right(radius));

            if (y_ == 0)
            if ((*this).down(radius) != nullptr && (*(*this).down(radius))->bid != bid)
            v.push_back((*this).right(radius));
            if (y_ = m.cols_ - 1)
            if ((*this).up(radius) != nullptr && (*(*this).up(radius))->bid != bid)
            v.push_back((*this).right(radius));
            else
            if ((*this).down(radius) != nullptr && (*(*this).down(radius))->bid != bid)
            v.push_back((*this).right(radius));
            if ((*this).up(radius) != nullptr && (*(*this).up(radius))->bid != bid)
            v.push_back((*this).right(radius));
            */

            switch ((**this).type)
            {
            case corner:
                switch ((*this).prevmove)
            case None:
                if (x_ == 0 && y_ == 0)
                    break;
                if (x_ == 0 && y != 0)
                    if ((*this).up(radius) != nullptr && (*(*this).up(radius))->bid != bid)
                    {
                        v.push_back((*this).right(radius));
                        break;
                    }

                if (x_ != 0 && y_ == 0)
                {
                    if ((*this).right(radius) != nullptr && (*(*this).right(radius))->bid != bid)
                        v.push_back((*this).right(radius));
                    if ((*this).left(radius) != nullptr && (*(*this).left(radius))->bid != bid)
                        v.push_back((*this).left(radius));
                    break;
                }
                else
                {
                    if ((*this).left(radius) != nullptr && (*(*this).left(radius))->bid != bid)
                        v.push_back((*this).left(radius));
                    if ((*this).up(radius) != nullptr && (*(*this).up(radius))->bid != bid)
                        v.push_back((*this).up(radius));
                }

                {
                }
            }
            if (v.empty())
                return iterator(false);

            else
            {
                auto it = v.back();
                v.pop_back();
                return it;
            }
        }

        //TODO
        void connect_buildings(int bid, int radius, Move prev)
        {
            Matrix<Vertex*>::iterator it1 = (*this);
            int count = 0;
            while (count != 2)
            {
                (**this)->bid = bid;
                switch ((**this)->type)
                {
                case wall:
                    moveClockWiseFromWall();
                    break;
                case corner:
                    if ((*this) != it1)
                        switch (prevmove)
                        {
                        case Right: down();  break;
                        case Left:  up();    break;
                        case Down:  left();  break;
                        case Up:    right(); break;
                        }
                    else
                    {
                        if (count == 0)
                            right();
                        count++;
                    }
                    break;
                case corner_inner:
                    switch (prevmove)
                    {
                    case Right: up();    break;
                    case Left:  down();  break;
                    case Down:  right(); break;
                    case Up:    left();  break;
                    }
                    break;
                }
            }

            while (radius != 1)
            {
                switch (prev)
                {
                case Right:
                    Vertex((*this).x_, (*this).y_);
                    (*(*this).left())->is_on_bridge = true;
                    (*this).left();
                    radius--;
                    break;
                case Left:
                    Vertex((*this).x_, (*this).y_);
                    (*(*this).right())->is_on_bridge = true;
                    (*this).right();
                    radius--;
                    break;
                case Up:
                    Vertex((*this).x_, (*this).y_);
                    (*(*this).down())->is_on_bridge = true;
                    (*this).down();
                    radius--;
                    break;
                case Down:
                    Vertex((*this).x_, (*this).y_);
                    (*(*this).up())->is_on_bridge = true;
                    (*this).up();
                    radius--;
                    break;
                }
            }
        }
    private:
        Matrix<T>& m_ = m;
        int x_ = 0;
        int y_ = 0;
        bool is_on_building = false;
    };
private:
    using M = vector <vector<Vertex*>>;
    static int max_bid;
    int  rows_;
    int  cols_;
    M    m_;
};

int Matrix<Vertex*>::max_bid = 0;
ifstream file("My_City.txt", std::ifstream::in);
Matrix<Vertex*> m;
char** hashdot;

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

Vertex::Vertex(int a, int b)
{
    x = a;
    y = b;
    type = Type::corner; // TODO: Test without this (relying only on in-class initializer)
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
            if (operator()(i, j) != nullptr)
                cout << 'o';
            else
                cout << '.';
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
                            it.move_Clockwise_From_Corner(start, count);
                            break;
                        case corner_inner:
                            it.move_Clockwise_From_Corner_Inner();
                            break;
                        }
                    }
                }
            m.rows_--;
            it.prevmove = None;
            if (m.rows_ > 1)
                it.right();
        }
        m.rows_ = r;
        m.cols_--;
        if (m.cols_ != 0)
            it = it.update(0, ++i);
    }
}

//TODO
template<class T>
void Matrix<T>::Circle()
{
    int count;
    int radius = 0;
    Matrix<Vertex*>::iterator it;
    Matrix<Vertex*>::iterator it2;
    Matrix<Vertex*>::iterator finder;
    std::vector<Matrix<Vertex*>::iterator> vertexes;
    int r = 0;

    finder.set_iterator();
    it = finder;
    it2 = it;

    while (max_bid != 0)
    {
        radius++;
        count = 0;
        it = it2;
        while (count != 2)
        {
            finder = it;
            //            if (finder.find_isolated_building(radius, (*finder)->bid, vertexes, finder.prevmove) != iterator(false))
            //            {
            //                finder.connect_buildings((*it)->bid, radius, it.prevmove);
            //              //  radius = 1;
            //            }
            switch ((*it)->type)
            {
            case wall:
                switch (it.prevmove)
                {
                case Right:  it.right(); break;
                case Left:   it.left();  break;
                case Down:   it.down();  break;
                case Up:     it.up();    break;
                }
                break;
            case corner:
                if (it != it2)
                    switch (it.prevmove)
                    {
                    case Right: it.down();  break;
                    case Left:  it.up();    break;
                    case Down:  it.left();  break;
                    case Up:    it.right(); break;
                    }
                else
                {
                    if (count == 0)
                        it.right();
                    count++;
                }
                break;
            case corner_inner:
                switch (it.prevmove)
                {
                case Right: it.up();    break;
                case Left:  it.down();  break;
                case Down:  it.right(); break;
                case Up:    it.left();  break;
                }
                break;
            }
        }
    }
}
#endif