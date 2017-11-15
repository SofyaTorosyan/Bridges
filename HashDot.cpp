#include "HashDot.h"

HashDot::HashDot(int l, int w) : length_(l), width_(w)
{ }

HashDot::HashDot() : length_(30), width_(30)
{ }

void HashDot::clear()
{
    hash_dot_.clear();
}

void HashDot::print()
{
    for (int i = 0; i < 30; i++)
        for (int j = 0; j < 30; j++)
            cout << hash_dot_[i][j];
    cout << endl;
}

int HashDot::width()
{
    return width_;
}

int HashDot::length()
{
    return length_;
}

int HashDot::row_size()
{
    return length_ + 1;
}

int HashDot::col_size()
{
    return width_ + 1;
}

char HashDot::up(int x, int y)
{
    if (y == 0)
        return '.';
    return hash_dot_[y - 1][x];
}

char HashDot::left(int x, int y)
{
    if (x == 0)
        return '.';
    return hash_dot_[y][x - 1];
}

char HashDot::upleft(int x, int y)
{
    if (y == 0 || x == 0)
        return '.';
    return hash_dot_[y - 1][x - 1];
}

char& HashDot::operator()(int i, int j)
{
    return  hash_dot_[j][i];
}

void HashDot::Random_Generate()
{
    cout << "City with random hashdots\n";
    for (int j = 0; j < width_; j++)
    {
        string line;
        hash_dot_.push_back(Random_Generate_String(line));
        cout << line << endl;
    }
}

void HashDot::Read_From_File(const std::string fileName = "My_City.txt")
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
    width_ = hash_dot_.size();
}

string& HashDot::Random_Generate_String(std::string& line)
{
    char str[20] = { '.','.','.','.','#','.','.','.','.','.' ,'.','.','.','.','.' ,'.','.','.','.','.' };
    for (int i = 0; i < length_; i++)
        line.push_back(str[rand() % 20]);
    return line;
}

