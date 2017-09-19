#ifndef HEADER_H
#define HEADER_H

#include <vector>
#include <string>
#include <chrono>
#include <utility>   /* std::pair */
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

using namespace std;

class Timer
{
private:
    std::chrono::steady_clock::time_point start;
public:
    Timer()
    {
        cout << "Time counting was started \n";
        start = std::chrono::steady_clock::now();
    }
    ~Timer()
    {
        auto end = std::chrono::steady_clock::now();
        cout << "\nTime  was " << std::chrono::duration_cast< std::chrono::milliseconds>(end - start).count() << " milliseconds\n";
    }
};

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
    int  count_for_corner_inners = 0;
    bool is_on_bridge            = false;
    bool is_on_bridge_end        = false;
    bool is_on_bridge_start      = false;
    Type type                    = Type::corner;
    Direction prev_direction     = None;
    Direction bridge_direction   = None;
};

class HashDot
{
private:
    vector<string> hash_dot_; 
    int    length_;
    int    width_;
 
public:
    HashDot(int l, int w) : length_(l), width_(w)  
    { }
    
    void clear()
    {
        hash_dot_.clear();
    }
    string& Random_Generate_String(std:: string& line)
    {
        char str[20] = { '.','.','.','.','#','.','.','.','.','.' ,'.','.','.','.','.' ,'.','.','.','.','.' };
        for (int i = 0; i < length_; i++)
            line.push_back(str[rand() % 20]);
        return line;
    }

    void Random_Generate()
    {
        cout << "City with random hashdots\n";
        for (int j = 0; j < width_; j++)
        {  
                string line;
                hash_dot_.push_back(Random_Generate_String(line));
                cout << line<< endl;
        }
    }
    
    void Read_From_File(const std::string fileName = "My_City.txt")
    {
        hash_dot_.clear();
        std::ifstream fileIn(fileName);
        if (!fileIn)
            cout << "No hashdot file " << fileName;

        std::string line;
        while (std::getline(fileIn, line))
        {
            hash_dot_.push_back(line);
            cout << line << endl; // create a separate function display();
        }

        length_ = hash_dot_.back().length();
        width_  = hash_dot_.size();
    }
   
    int length()
    {
        return length_;
    }

    int width()
    {
        return width_;
    }
   
    char& operator()(int i, int j)
    {
        return  hash_dot_[j][i];
    }
 
    int row_size()
    {
        return length_ + 1;
    }

    int col_size()
    {
        return width_ + 1;
    }
};

template<class T>
class Matrix
{
public:
    Matrix();
    ~Matrix();
    void C4V(int, int);
    int  rows();
    int  cols();
    void print();
    void Circle();
    Vertex*& operator() (int, int);
    void Create_Vertexes();
    void Write_To_File(string);
    void enumerateBuildings();

    class iterator
    {
    public:
        using vertex_pointer = T;
        using reference      = T&;
        using pointer        = T*;
        iterator() = default;

        iterator(bool b) : is_on_building_(b) { }

        int x() { return x_; }

        int y() { return y_; }

        void set_iterator()
        {
            int r = m.rows_;
            int c = m.cols_;
            int i = 0;
            while (c != 1)
            {
                while (r != 1)
                {
                    if (**this == nullptr)
                        *this->move_right();
                    else
                        return;
                    r--;
                }
                *this->update(0, ++i);
                c--;
                r = m.rows_;
            }
        }

        iterator move_left(int i = 1)
        {
            (*this).x_ -= i;
            prevmove_ = Left;
            return *this;
        }

        iterator move_right(int i = 1)
        {
            (*this).x_ += i;
            prevmove_ = Right;
            return *this;
        }

        iterator move_up(int i = 1)
        {
            (*this).y_ -= i;
            prevmove_ = Up;
            return *this;
        }

        iterator move_down(int i = 1)
        {
            (*this).y_ += i;
            prevmove_ = Down;
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

        void  move_Opposite_To_Bridge_Direction(Direction bridge_direction)
        {
            switch (bridge_direction)
            {
            case Right:
                move_left();
                while (!(**this)->is_on_bridge_end && !(**this)->is_on_bridge_start)
                    move_left();
                break;
            case Left:
                move_right();
                while (!(**this)->is_on_bridge_end && !(**this)->is_on_bridge_start)
                    move_right();
                break;
            case Up:
                move_down();
                while (!(**this)->is_on_bridge_end && !(**this)->is_on_bridge_start)
                    move_down();
                break;
            case Down:
                move_up();
                while (!(**this)->is_on_bridge_end && !(**this)->is_on_bridge_start)
                    move_up();
                break;
            }
        }

        void move_Towards_To_Bridge_Direction(Direction bridge_direction)
        {
            switch (bridge_direction)
            {
            case Right:
                move_right();
                while (!(**this)->is_on_bridge_end && !(**this)->is_on_bridge_start)
                    move_right();
                break;
            case Left:
                move_left();
                while (!(**this)->is_on_bridge_end && !(**this)->is_on_bridge_start)
                    move_left();
                break;
            case Up:
                move_up();
                while (!(**this)->is_on_bridge_end && !(**this)->is_on_bridge_start)
                    move_up();
                break;
            case Down:
                move_down();
                while (!(**this)->is_on_bridge_end && !(**this)->is_on_bridge_start)
                    move_down();
            }
        }

        void move_Clockwise_From_Wall()
        {
            switch ((**this)->prev_direction)
            {
            case Up:    move_up();    (**this)->prev_direction = Up;    break;
            case Down:  move_down();  (**this)->prev_direction = Down;  break;
            case Left:  move_left();  (**this)->prev_direction = Left;  break;
            case Right: move_right(); (**this)->prev_direction = Right; break;
            }
        }

        void move_From_Wall()
        {
            if (!(**this)->is_on_bridge_start)
            {
                if ((**this)->is_on_bridge_end && ++(**this)->bridge_end == 2)
                {
                    (**this)->bridge_end = 0;
                    move_Opposite_To_Bridge_Direction((**this)->bridge_direction);
                }
                else
                    move_Clockwise_From_Wall();
            }
            else
            {
                if ((**this)->is_on_bridge_start && (++(**this)->bridge_start == 1))
                    move_Towards_To_Bridge_Direction((**this)->bridge_direction);
                else
                {
                    if ((**this)->bridge_start == 2)
                        move_Clockwise_From_Wall();
                    else
                        move_Opposite_To_Bridge_Direction(find(prev_bridge_directions, *this));
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
                    move_Opposite_To_Bridge_Direction((**this)->bridge_direction);
                }
                else
                {
                    if ((**this)->is_on_bridge_end && (**this)->prev_direction == None)
                        move_Clockwise_From_Corner(start, count);
                    else
                        move_Clockwise_From_Corner(start, count);
                    return;
                }
            }
            else
            {
                if (++(**this)->bridge_start == 1)
                    move_Towards_To_Bridge_Direction((**this)->bridge_direction);
                else
                {
                    if ((**this)->bridge_start == 2)
                        move_Clockwise_From_Corner(start, count);
                    else
                        move_Opposite_To_Bridge_Direction(find(prev_bridge_directions, *this));
                }
            }
        }

        void move_Clockwise_From_Corner(iterator start, int& count)
        {
            if ((**this)->prev_direction != None)   /* Still it haven't come to the beginning.*/
            {
                if ((*this) != start)
                    switch ((**this)->prev_direction)
                    {
                    case Up:    move_right(); break;
                    case Down:  move_left();  break;
                    case Left:  move_up();    break;
                    case Right: move_down();  break;
                    }
                else
                {
                    if (++count == 1)
                        switch ((**this)->prev_direction)
                        {
                        case Up:    move_right(); break;
                        case Down:  move_left();  break;
                        case Left:  move_up();    break;
                        case Right: move_down();  break;
                        }
                }
            }
            else
            {
                if ((*this) == start)
                    count++;
                if (count != 2)
                    move_right();
            }
            return;
        }

        void move_Clockwise_From_Corner_Inner()
        {
            switch (this->prevmove_)
            {
            case Right: move_up();    break;
            case Left:  move_down();  break;
            case Down:  move_right(); break;
            case Up:    move_left();  break;
            }
        }

        void move_Clockwise_From_Inside(int& count)
        {
            (**this)->bid = (*(*this).left_v())->bid;
            max_bid--;
            count = 2;         /* Go out prevmove_ while.*/
        }

        iterator& operator=(iterator& r_it)
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
        int x_ = 0;
        int y_ = 0;
        Matrix<T>& m_ = m;
        bool is_on_building_ = false;
        Direction prevmove_  = None;
    };

private:
    using M = vector <vector<Vertex*>>;
    static int max_bid;
    int  rows_;
    int  cols_;
    M    m_;
};

int radius = 0; 
int Matrix<Vertex*>::max_bid = 0;
HashDot  hash_dot(100, 80); 
ofstream fileOut("Connected.txt");
Matrix<Vertex*> m;
vector<pair<Vertex*, Direction>> symmetric_corner_inners;
vector<pair<Matrix<Vertex*>::iterator, Direction>> prev_bridge_directions;
Direction prev_end = None;
Direction prev_start = None;
int count_of_bridges = 0;
int non_connected_buildings = 0;

bool contains(Vertex* v)
{
    return (m((*v).x, (*v).y)) != nullptr;
}

bool is_symmetric_corner_inner(Matrix<Vertex*>::iterator vertex)
{
    if ((hash_dot((**vertex).x,     (**vertex).y - 1) == '.' && hash_dot((**vertex).x - 1, (**vertex).y) == '.')
     || (hash_dot((**vertex).x - 1, (**vertex).y - 1) == '.' && hash_dot((**vertex).x,     (**vertex).y) == '.'))
        return true;
    return false;

}

Direction find(vector<pair<Matrix<Vertex*>::iterator, Direction>> prev_bridge_directions, Matrix<Vertex*>::iterator it)
{
    for (unsigned int i = 0; i < prev_bridge_directions.size(); i++)
        if (prev_bridge_directions[i].first == it)
            return prev_bridge_directions[i].second;
}

void connect_2_building(pair<Matrix<Vertex*>::iterator, char>& connectible, int bid)
{
    int i = 0;
    int count = 0;
    pair<Matrix<Vertex*>::iterator, char> connectible_start = connectible;

    if ((**connectible.first).bid != bid)
        while (count != 2)
        {
            switch ((*connectible.first)->type)
            {
            case wall:
                (**connectible.first).bid = bid;
                if (connectible.first == connectible_start.first)
                    ++count;
                if (count != 2)
                    connectible.first.move_Clockwise_From_Wall();
                break;
            case corner:
                (**connectible.first).bid = bid;
                connectible.first.move_Clockwise_From_Corner(connectible_start.first, count);
                break;
            case corner_inner:
                (**connectible.first).bid = bid;
                connectible.first.move_Clockwise_From_Corner_Inner();
                break;
            }
        }

    for (unsigned int i = 0; i < symmetric_corner_inners.size(); i++)
        m(symmetric_corner_inners.at(i).first->x, symmetric_corner_inners.at(i).first->y)->prev_direction = symmetric_corner_inners.at(i).second;

    switch (connectible.second)
    {
    case 'R':
        (**connectible.first).is_on_bridge_end = true;
        if ((**connectible.first).is_on_bridge_start)
        {
            (**connectible.first).is_on_bridge_start = false;
            prev_bridge_directions.push_back(make_pair(connectible.first, (**connectible.first).bridge_direction));
        }
        (**connectible.first).bridge_direction = Right;
        connectible.first.move_left();
        while (*connectible.first == nullptr || (*connectible.first != nullptr && (**connectible.first).is_on_bridge == true))
        {
            auto v = new Vertex(connectible.first.x(), connectible.first.y());
            m(connectible.first.x(), connectible.first.y()) = v;
            (*connectible.first)->is_on_bridge = true;

            m(connectible.first.x(), connectible.first.y())->prev_direction = Right;
            connectible.first.move_left();
        }
        (**connectible.first).is_on_bridge_start = true;
        if ((**connectible.first).is_on_bridge_end)
        {
            (**connectible.first).is_on_bridge_end = false;
            prev_bridge_directions.push_back(make_pair(connectible.first, (**connectible.first).bridge_direction));
        }
        (**connectible.first).bridge_direction = Right;
        break;
    case 'L':
        (**connectible.first).is_on_bridge_end = true;
        if ((**connectible.first).is_on_bridge_start)
        {
            (**connectible.first).is_on_bridge_start = false;
            prev_bridge_directions.push_back(make_pair(connectible.first, (**connectible.first).bridge_direction));
        }
        (**connectible.first).bridge_direction = Left;
        connectible.first.move_right();
        while (*connectible.first == nullptr || (*connectible.first != nullptr && (**connectible.first).is_on_bridge == true))
        {
            auto v = new Vertex(connectible.first.x(), connectible.first.y());
            m(connectible.first.x(), connectible.first.y()) = v;
            (*connectible.first)->is_on_bridge = true;

            m(connectible.first.x(), connectible.first.y())->prev_direction = Left;
            connectible.first.move_right();
        }
        (**connectible.first).is_on_bridge_start = true;
        if ((**connectible.first).is_on_bridge_end)
        {
            (**connectible.first).is_on_bridge_end = false;
            prev_bridge_directions.push_back(make_pair(connectible.first, (**connectible.first).bridge_direction));
        }
        (**connectible.first).bridge_direction = Left;
        break;
    case 'U':
        (**connectible.first).is_on_bridge_end = true;
        if ((**connectible.first).is_on_bridge_start)
        {
            (**connectible.first).is_on_bridge_start = false;
            prev_bridge_directions.push_back(make_pair(connectible.first, (**connectible.first).bridge_direction));
        }
        (**connectible.first).bridge_direction = Up;
        connectible.first.move_down();
        while (*connectible.first == nullptr || (*connectible.first != nullptr && (**connectible.first).is_on_bridge == true))
        {
            auto v = new Vertex(connectible.first.x(), connectible.first.y());
            m(connectible.first.x(), connectible.first.y()) = v;
            (*connectible.first)->is_on_bridge = true;

            m(connectible.first.x(), connectible.first.y())->prev_direction = Up;
            connectible.first.move_down();
        }
        (**connectible.first).is_on_bridge_start = true;
        if ((**connectible.first).is_on_bridge_end)
        {
            (**connectible.first).is_on_bridge_end = false;
            prev_bridge_directions.push_back(make_pair(connectible.first, (**connectible.first).bridge_direction));
        }
        (**connectible.first).bridge_direction = Up;
        break;

    case 'D':
        (**connectible.first).is_on_bridge_end = true;
        if ((**connectible.first).is_on_bridge_start)
        {
            (**connectible.first).is_on_bridge_start = false;
            prev_bridge_directions.push_back(make_pair(connectible.first, (**connectible.first).bridge_direction));
        }
        (**connectible.first).bridge_direction = Down;
        connectible.first.move_up();
        while (*connectible.first == nullptr || (*connectible.first != nullptr && (**connectible.first).is_on_bridge == true))
        {
            auto v = new Vertex(connectible.first.x(), connectible.first.y());
            m(connectible.first.x(), connectible.first.y()) = v;
            (*connectible.first)->is_on_bridge = true;

            m(connectible.first.x(), connectible.first.y())->prev_direction = Down;
            connectible.first.move_up();
        }
        (**connectible.first).is_on_bridge_start = true;
        if ((**connectible.first).is_on_bridge_end)
        {
            (**connectible.first).is_on_bridge_end = false;
            prev_bridge_directions.push_back(make_pair(connectible.first, (**connectible.first).bridge_direction));
        }
        (**connectible.first).bridge_direction = Down;
        break;
    }
}

pair<Matrix<Vertex*>::iterator, char> find_Isolated_Building(pair<Matrix<Vertex*>::iterator, char> it, int bid)
{
    if ((**it.first).x == 0)
    {
        if (((**it.first).x + radius) < m.rows())
            if ((*it.first.right_v(radius)) != nullptr)
                if ((**it.first.right_v(radius)).bid != (**it.first).bid && (**it.first.right_v(radius)).bid != 0 && (**it.first.right_v(radius)).type != inside)
                {
                    it.first.move_right(radius);
                    (it.second) = 'R';
                    return it;
                }
    }

    if ((**it.first).x == m.rows() - 1)
    {
        if (((**it.first).x - radius) > 0)
            if ((*it.first.left_v(radius)) != nullptr)
                if ((**it.first.left_v(radius)).bid != (**it.first).bid && (**it.first.left_v(radius)).bid != 0 && (**it.first.left_v(radius)).type != inside)
                {
                    it.first.move_left(radius);
                    (it.second) = 'L';
                    return it;
                }
    }

    else
    {
        if ((**it.first).x + radius < m.rows())
            if ((*it.first.right_v(radius)) != nullptr)
                if ((**it.first.right_v(radius)).bid != (**it.first).bid && (**it.first.right_v(radius)).bid != 0 && (**it.first.right_v(radius)).type != inside)
                {
                    it.first.move_right(radius);
                    (it.second) = 'R';
                    return it;
                }

        if (((**it.first).x - radius) > 0)
            if ((*it.first.left_v(radius)) != nullptr)
                if ((**it.first.left_v(radius)).bid != (**it.first).bid && (**it.first.left_v(radius)).bid != 0 && (**it.first.left_v(radius)).type != inside)
                {
                    it.first.move_left(radius);
                    (it.second) = 'L';
                    return it;
                }
    }

    if ((**it.first).y == 0)
    {
        if (((**it.first).y + radius) < m.cols())
            if ((*it.first.down_v(radius)) != nullptr)
                if ((**it.first.down_v(radius)).bid != (**it.first).bid && (**it.first.down_v(radius)).bid != 0 && (**it.first.down_v(radius)).type != inside)
                {
                    it.first.move_down(radius);
                    (it.second) = 'D';
                    return it;
                }
    }

    if ((**it.first).y == m.cols() - 1)
    {
        if (((**it.first).y - radius) > 0)
            if ((*it.first.up_v(radius)) != nullptr)
                if ((**it.first.up_v(radius)).bid != (**it.first).bid && (**it.first.up_v(radius)).bid != 0 && (**it.first.up_v(radius)).type != inside)
                {
                    it.first.move_up(radius);
                    (it.second) = 'U';
                    return it;
                }
    }

    else
    {
        if (((**it.first).y + radius) < m.cols())
            if ((*it.first.down_v(radius)) != nullptr)
                if ((**it.first.down_v(radius)).bid != (**it.first).bid && (**it.first.down_v(radius)).bid != 0 && (**it.first.down_v(radius)).type != inside)
                {
                    it.first.move_down(radius);
                    (it.second) = 'D';
                    return it;
                }

        if (((**it.first).y - radius) > 0)
            if ((*it.first.up_v(radius)) != nullptr)
                if ((**it.first.up_v(radius)).bid != (**it.first).bid && (**it.first.up_v(radius)).bid != 0 && (**it.first.up_v(radius)).type != inside)
                {
                    it.first.move_up(radius);
                    (it.second) = 'U';
                    return it;
                }
    }
    return pair<Matrix<Vertex*>::iterator, char>();
}

void find_new_building(Matrix<Vertex*>::iterator& it)
{
    int bid = (**it).bid;
    int rows = (**it).x;
    int cols = (**it).y;
    while (cols != m.cols() - 1)
    {
        while (rows != m.rows() - 1)
        {
            if ((*it) != nullptr)
                if ((**it).bid != bid && (**it).bid != 0)
                    return;
                else
                {
                    rows++;
                    it.move_right();
                }
            else
            {
                it.move_right();
                rows++;
            }
        }
        it.update(0, ++cols);
        rows = 0;
    }
}

Vertex::Vertex(int a, int b)
{
    x = a;
    y = b;
    bid = 0;
}

template<class T>
Matrix<T>::Matrix() : rows_(hash_dot.length() + 1), cols_(hash_dot.width() + 1)
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
    cout << "City with bridges is\n";
    for (int j = 0; j < cols_; j++)
    {
        for (int i = 0; i < rows_; i++)
        {
            if (m(i, j) == nullptr)
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

template<class T>
void Matrix<T>::Write_To_File(string CFile)
{
    ofstream C_File;
    C_File.open("CFile");
    for (int j = 0; j < cols_; j++)
    {
        for (int i = 0; i < rows_; i++)
        {
            if (m(i, j) == nullptr)
                fileOut << " ";
            else
            {
                if ((*m(i, j)).is_on_bridge)
                    fileOut << '.';
                else

                    fileOut << 'o';
            }
        }
        fileOut << '\n';
    }
}

char left(int x, int y)
{
    if (x == 0)
        return '.';
    return hash_dot(x - 1, y);
}

char up(int x, int y)
{
    if (y == 0)
        return '.';
    return hash_dot(x, y - 1);
}

char upleft(int x, int y)
{
    if (y == 0 || x == 0)
        return '.';
    return hash_dot(x - 1, y - 1);
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
    max_bid = 0;
    int i = 0;
    while (m.cols_ != 0)
    {
        while (m.rows_ != 0)
        {
            if ((*it) != nullptr)
                if ((*it)->bid == 0)
                {
                    start = it;
                    max_bid++;
                    int count = 0;            /* For knowing that it came to the beginning again. */
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
                            if (it != start)    /* Still it haven't come to the beginning. */
                                switch ((**it).prev_direction)
                                {
                                case Up:     it.move_right(); (**it).prev_direction = Right;  break;
                                case Down:   it.move_left();  (**it).prev_direction = Left;   break;
                                case Left:   it.move_up();    (**it).prev_direction = Up;     break;
                                case Right:  it.move_down();  (**it).prev_direction = Down;   break;
                                }
                            else                /* When it is at the beginning of building.(prev_direction=-1) */
                            {
                                (**it).prev_direction = None;
                                if (count == 0)
                                {
                                    it.move_right();
                                    (**it).prev_direction = Right;
                                }
                                count++;
                            }
                            break;
                        case corner_inner:
                            switch ((**it).prev_direction)
                            {
                            case Up:
                                it.move_left();
                                (**it).prev_direction = Left;
                                break;
                            case Down:
                                if (is_symmetric_corner_inner(it))
                                    symmetric_corner_inners.push_back(make_pair((*it), Down));
                                it.move_right(); (**it).prev_direction = Right;
                                break;
                            case Left:
                                it.move_down();
                                (**it).prev_direction = Down;
                                break;
                            case Right:
                                if (is_symmetric_corner_inner(it))
                                    symmetric_corner_inners.push_back(make_pair((*it), Right));
                                it.move_up();
                                (**it).prev_direction = Up;
                                break;
                            }
                            break;
                        }
                    }

                    for (unsigned int i = 0; i < symmetric_corner_inners.size(); i++)
                        m(symmetric_corner_inners.at(i).first->x, symmetric_corner_inners.at(i).first->y)->prev_direction = symmetric_corner_inners.at(i).second;
                }
            m.rows_--;
            if (m.rows_ > 1)
                it.move_right();
        }
        m.rows_ = r;
        m.cols_--;
        if (m.cols_ != 0)
            it = it.update(0, ++i);
    }
    /* Recover m.rows_ and m.cols_ values. */
    m.rows_ = r;
    m.cols_ = c;
}

template<class T>
void Matrix<T>::Circle()
{
    int count;
    int r = 0;
    radius = 0;

    Matrix<Vertex*>::iterator             circle_start;
    pair<Matrix<Vertex*>::iterator, char> it;
    pair<Matrix<Vertex*>::iterator, char> connectible;
    vector< Matrix<Vertex*>::iterator > bridge_starts;
    vector< Matrix<Vertex*>::iterator > bridge_ends;

    it.first.set_iterator();
    circle_start = it.first;
    connectible.first = circle_start;
    while (it.first.x() != rows_ - 1 && it.first.y() != cols_ - 1 && max_bid != 1)
    {
        while (radius != max(m.rows_, m.cols_) && max_bid != 1)
        {
            it.first = circle_start;
            radius++;
            count = 0;
            while (count != 2)
            {
                connectible = find_Isolated_Building(it, (**it.first).bid);
                if (connectible.first != pair<iterator, char>().first && connectible.second != pair<iterator, char>().second && (**it.first).bid != (**connectible.first).bid && (**it.first).bid != 0)
                {

                    bridge_starts.push_back(it.first);
                    bridge_ends.push_back(connectible.first);
                    connect_2_building(connectible, (**circle_start).bid);
                    count = 0;
                    radius = 1;
                    for (unsigned int i = 0; i < symmetric_corner_inners.size(); i++)
                        m(symmetric_corner_inners.at(i).first->x, symmetric_corner_inners.at(i).first->y)->count_for_corner_inners = 0;
                    it.first = circle_start;
                    max_bid--;
                    count_of_bridges++;

                    for (unsigned int i = 0; i < bridge_starts.size(); i++)
                        (**bridge_starts[i]).bridge_start = 0;

                    for (unsigned int i = 0; i < bridge_ends.size(); i++)
                        (**bridge_ends[i]).bridge_end = 0;
                }
                if (max_bid != 1)

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
                else
                    count = 2;
            }

            for (unsigned int i = 0; i < symmetric_corner_inners.size(); i++)
                m(symmetric_corner_inners.at(i).first->x, symmetric_corner_inners.at(i).first->y)->prev_direction = symmetric_corner_inners.at(i).second;

            for (unsigned int i = 0; i < bridge_starts.size(); i++)
                (**bridge_starts[i]).bridge_start = 0;
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

template<class T>
void Matrix<T>::Create_Vertexes()
{
    rows_ = hash_dot.length() + 1;
    cols_ = hash_dot.width() + 1;
    for (int y = 0; y < hash_dot.width(); y++)
        for (int x = 0; x < hash_dot.length(); x++)
            if (hash_dot(x, y ) == '#')
                m.C4V(x, y);
    cout << endl;
}

void clean_all()
{
    for (int j = 0; j < m.cols(); j++)
        for (int i = 0; i < m.rows(); i++)
            m(i, j) = nullptr;
    symmetric_corner_inners.clear();
    prev_bridge_directions.clear();
    count_of_bridges        = 0;
    non_connected_buildings = 0;
    prev_end                = None;
    prev_start              = None;
    hash_dot.clear();
}
#endif