#ifndef MATRIX_H
#define MATRIX_H

#include "Timer.h"
#include "HashDot.h"
#include "Vertex.h"

template<class T>
class Matrix
{
public:
    HashDot  hash_dot; // size-24,align-4
    Matrix();
    ~Matrix();
    void C4V(int, int);
    int  rows()  const;
    int  cols()  const;
    void print() const;
    void Circle();
    Vertex*& operator() (int, int);
    void  Create_Vertexes();
   static void Write_To_File(const string&);
    void enumerateBuildings();
    int  Disconnected();

    class iterator
    {
    public:
        using vertex_pointer = T;
        using reference = T&;
        using pointer = T*;
        iterator() = default;
        explicit iterator(bool);
        iterator(int, int);
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
        void move_From_Corner(iterator, int&);
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
    bool is_symmetric_corner_inner(iterator&);
private:
    using M = vector <vector<Vertex*>>;
    static int max_bid;
    int  rows_; //size-4
    int  cols_; //size-4
    M    m_;   // size-16, align-4
};

template<class T>
Matrix<T>::Matrix() : rows_(hash_dot.length() + 1), cols_(hash_dot.width() + 1)
{
    m_.resize(rows_);
    for (auto& col : m_)
        col.resize(cols_);
}


template<class T>
bool Matrix<T>::is_symmetric_corner_inner(iterator& vertex)
{
    if ((hash_dot((**vertex).x, (**vertex).y - 1) == '.' && hash_dot((**vertex).x - 1, (**vertex).y) == '.')
        || (hash_dot((**vertex).x - 1, (**vertex).y - 1) == '.' && hash_dot((**vertex).x, (**vertex).y) == '.'))
        return true;
    return false;
}

template<class T>
Matrix<T>::~Matrix()
{
    for (int j = 0; j < m.cols_; j++)
        for (int i = 0; i < m.rows_; i++)
            delete m(i, j);
}

template<class T>
int Matrix<T>::rows() const
{
    return rows_;
}

template<class T>
int Matrix<T>::cols() const
{
    return cols_;
}

template<class T>
void Matrix<T>::print() const
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
int Matrix<T>::Disconnected()
{
    vector<int> v;
    int count = 0;
    bool no_building = false;
   
    for (int j = 0; j < m.cols(); j++)
    {
        for (int i = 0; i < m.rows(); i++)
        {
            if (m(i, j) != nullptr)
            {
                no_building = true;
                if ((*m(i, j)).bid != 0 && (*m(i, j)).type != inside)
                    if (std::find(v.begin(), v.end(), (*m(i, j)).bid) == v.end())
                    {
                        ++count;
                        v.push_back((*m(i, j)).bid);
                       
                    }
            }
        }
    }
    return  (no_building == true) ? count : --count;
}

template<class T>
 void Matrix<T>::Write_To_File(const string& file_name)
{
    ofstream B_File(file_name);
    for (int j = 0; j < m.cols(); j++)
    {
        for (int i = 0; i < m.rows(); i++)
        {
            if (m(i, j) == nullptr)
                B_File << " ";
            else
            {
                if ((*m(i, j)).is_on_bridge || (*m(i, j)).type == inside)
                    B_File << '.';
                else

                    B_File << 'o';
            }
        }
        B_File << '\n';
    }

    B_File << "-----------------------\nNumber of bridges.... " << count_of_bridges << endl;
    B_File << "Total length......... " << bridge_length << endl;
    B_File << "Disconnected groups.. " << non_connected_buildings << "\n----------------------- \n";
}


template<class T>
void Matrix<T>::C4V(int x, int y)
{
    auto v = new Vertex[4];
    v[0] = { x,   y };
    v[1] = { x,   y + 1 };
    v[2] = { x + 1, y + 1 };
    v[3] = { x + 1, y };

    if (hash_dot.left(x, y) == '#')
    {
        v[0].type = wall;
        v[1].type = wall;
    }

    if (hash_dot.up(x, y) == '#')
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
        if (hash_dot.left(x, y) == '#' || hash_dot.up(x, y) == '#')
        {
            switch ((*m(x, y)).type)
            {
            case corner_inner:
            {
                if (hash_dot.upleft(x, y) == '#')
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

            if (hash_dot.up(x, y) == '#')
                if ((*m(x + 1, y)).type == wall)
                    v[3].type = corner_inner;
                else
                    v[3].type = wall;

            if (hash_dot.left(x, y) == '#')
                v[1].type = wall;
        }

        else
            if (m(x, y) != nullptr)
                v[0].type = corner_inner;

        if (hash_dot.up(x, y) != '#')
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
    Matrix<Vertex*>::iterator it(0,0);
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
    cout << endl<<"Count of cities "<<max_bid;
    int count;
   
    connected_bid.push_back(0);
    Matrix<Vertex*>::iterator             circle_start;
    pair<Matrix<Vertex*>::iterator, char> it;
    pair<Matrix<Vertex*>::iterator, char> connectible;
    vector< Matrix<Vertex*>::iterator> bridge_starts;
    vector< Matrix<Vertex*>::iterator> bridge_ends;

    non_connected_buildings = max_bid;
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
                    connected_bid.push_back((*it.first)->bid);
                    bridge_length += radius;
                    bridge_starts.push_back(it.first);
                    bridge_ends.push_back(connectible.first);
                    connect_2_building(connectible, (**circle_start).bid);
                    count  = 0;
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
            connected_bid.push_back((*it.first)->bid);
            max_bid--;
            find_new_building(it.first);
            circle_start = it.first;
            radius = 0;
        }
    }
    non_connected_buildings = Disconnected();
}


template<class T>
void Matrix<T>::Create_Vertexes()
{
    rows_ = hash_dot.length() + 1;
    cols_ = hash_dot.width() + 1;
    for (int y = 0; y < hash_dot.width(); y++)
        for (int x = 0; x < hash_dot.length(); x++)
            if (hash_dot(x, y) == '#')
                m.C4V(x, y);

    cout << endl;
}


template<class T>
Matrix<T>::iterator::iterator(bool b) { is_on_building_ = b; }

template<class T>
Matrix<T>::iterator::iterator(int x, int y) : x_(x), y_(y) 
{
    cout << '\a';
}

template<class T>
int Matrix<T>::iterator::x() { return x_; }

template<class T>
int Matrix<T>::iterator::y() { return y_; }

template<class T>
void Matrix<T>::iterator::set_iterator()
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

template<class T>
typename Matrix<T>::iterator Matrix<T>::iterator::move_left(int i = 1)
{
    (*this).x_ -= i;
    prevmove_ = Left;
    return *this;
}

template<class T>
typename Matrix<T>::iterator Matrix<T>::iterator::move_right(int i = 1)
{
    (*this).x_ += i;
    prevmove_ = Right;
    return *this;
}

template<class T>
typename Matrix<T>::iterator Matrix<T>::iterator::move_up(int i = 1) {
    (*this).y_ -= i;
    prevmove_ = Up;
    return *this;
}


template<class T>
typename Matrix<T>::iterator Matrix<T>::iterator::move_down(int i = 1)
{
    (*this).y_ += i;
    prevmove_ = Down;
    return *this;
}

template<class T>
typename Matrix<T>::iterator Matrix<T>::iterator::left_v(int i = 1)
{
    iterator temp = *this;
    temp.x_ -= i;
    return temp;
}

template<class T>
typename Matrix<T>::iterator Matrix<T>::iterator::right_v(int i = 1)
{
    iterator temp = *this;
    temp.x_ += i;
    return temp;
}

template<class T>
typename Matrix<T>::iterator Matrix<T>::iterator::up_v(int i = 1)
{
    iterator temp = *this;
    temp.y_ -= i;
    return temp;
}

template<class T>
typename Matrix<T>::iterator Matrix<T>::iterator::down_v(int i = 1)
{
    iterator temp = *this;
    temp.y_ += i;
    return temp;
}

template<class T>
typename bool Matrix<T>::iterator::operator!=(const iterator& r_it)
{
    return (x_ != r_it.x_ || y_ != r_it.y_);
}


template<class T>
typename bool Matrix<T>::iterator::operator==(const iterator& r_it)
{
    return (x_ == r_it.x_ && y_ == r_it.y_);
}


template<class T>
void Matrix<T>::iterator::move_Opposite_To_Bridge_Direction(Direction bridge_direction)
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


template<class T>
void Matrix<T>::iterator::move_Towards_To_Bridge_Direction(Direction bridge_direction)
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


template<class T>
void Matrix<T>::iterator::move_Clockwise_From_Wall()
{
    switch ((**this)->prev_direction)
    {
    case Up:    move_up();    (**this)->prev_direction = Up;    break;
    case Down:  move_down();  (**this)->prev_direction = Down;  break;
    case Left:  move_left();  (**this)->prev_direction = Left;  break;
    case Right: move_right(); (**this)->prev_direction = Right; break;
    }
}




template<class T>
void Matrix<T>::iterator::move_From_Wall()
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


template<class T>
void Matrix<T>::iterator::move_From_Corner(iterator start, int& count) {
    if ((**this)->bid_count == 2)
        switch ((*this).prevmove_)
        {
        case Left:
            do
                move_down();
            while ((**this)->bid == 0);
            break;
        case Right:
            do
                move_up();
            while ((**this)->bid == 0);
            break;
        case Up:
            do
                move_left();
            while ((**this)->bid == 0);
            break;
        case Down:
            do
                move_right();
            while ((**this)->bid == 0);
            break;
        }
    else
        if (!(**this)->is_on_bridge_start)
        {
            if ((**this)->is_on_bridge_end && ++(**this)->bridge_end == 2)
            {
                if ((*this)!= start)
                {
                    (**this)->bridge_end = 0;
                    move_Opposite_To_Bridge_Direction((**this)->bridge_direction);
                }
                else
                {
                    count = 2;
                }
            }
            else
            {
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



template<class T>
void Matrix<T>::iterator::move_Clockwise_From_Corner(iterator start, int& count)
{
    if ((**this)->prev_direction != None)   /* Still it haven't come to the beginning.*/
    {
        if ((*this) != start)
            switch ((**this)->prev_direction)
            {
            case Up:    move_right();  break;
            case Down:  move_left();   break;
            case Left:  move_up();     break;
            case Right: move_down();   break;
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

template<class T>
void Matrix<T>::iterator::move_Clockwise_From_Corner_Inner()
{
    switch (this->prevmove_)
    {
    case Right: move_up();    break;
    case Left:  move_down();  break;
    case Down:  move_right(); break;
    case Up:    move_left();  break;
    }
}

template<class T>
void Matrix<T>::iterator::move_Clockwise_From_Inside(int& count)
{
    (**this)->bid = (*(*this).left_v())->bid;
    max_bid--;
    count = 2;         /* Go out prevmove_ while.*/
}

template<class T>
typename Matrix<T>::iterator& Matrix<T>::iterator::operator=(iterator& r_it)
{
    x_ = r_it.x_;
    y_ = r_it.y_;
    return *this;
}

template<class T>
typename Matrix<T>::iterator::reference Matrix<T>::iterator::operator* ()
{
    return m_(x_, y_);
}

template<class T>
typename Matrix<T>::iterator::pointer Matrix<T>::iterator:: operator->() { return &m_(x_, y_); }


template<class T>
bool Matrix<T>::iterator::is_valid()
{
    if (x_ > m_.col_ || y_ > m_.row_)
        return false;
    else
        return m_(x_, y_) != nullptr;
}

template<class T>
typename Matrix<T>::iterator Matrix<T>::iterator::update(int x, int y)
{
    x_ = x;
    y_ = y;
    return *this;
}

#endif

