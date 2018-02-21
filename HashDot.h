#ifndef HASHDOT_H
#define HASHDOT_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

class HashDot
{
private:  
    vector<string> hash_dot_;  // size=16, align=4,   sizeof(HashDot) = 24, alignof(hashdot)=4
    int            length_;
    int            width_;
    
public:
    HashDot();
    HashDot(int, int);
    void clear();
    void print() const;
    int  width() const;
    int  length()const;
    int  row_size()const;
    int  col_size()const;
    char up         (int, int) const;
    char left       (int, int) const;   
    char upleft     (int, int) const;
   inline char& operator()(int, int); 
    void Random_Generate();
    void Read_From_File(const std::string&);
    string& Random_Generate_String(std::string&);  
};

HashDot::HashDot(int l, int w) : length_(l), width_(w)
{ }

HashDot::HashDot() : length_(100), width_(100) 
{ }

void HashDot::clear()
{
    hash_dot_.clear();
}

inline void HashDot::print() const
{ 
    for (int i = 0; i < 100; i++) 
        for (int j = 0; j < 100; j++)
            cout << hash_dot_[i][j];
    cout << endl;
} 
   
int HashDot::width() const
{
    return width_;
}

int HashDot::length() const
{
    return length_;
}  

int HashDot::row_size() const
{
    return length_ + 1;
}

int HashDot::col_size() const
{
    return width_ + 1;
}

char HashDot::up(int x, int y) const
{
    if (y == 0)
        return '.';
    return hash_dot_[y - 1][x];
}

char HashDot::left(int x, int y) const
{
    if (x == 0)
        return '.';
    return hash_dot_[y][x - 1];
}

char HashDot::upleft(int x, int y) const
{
    if (y == 0 || x == 0)
        return '.';
    return hash_dot_[y - 1][x - 1];
}

inline char& HashDot::operator()(int i, int j)
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
       // cout << line << endl;
    }
}

void HashDot::Read_From_File(const std::string& fileName = "My_City.txt")
{
    hash_dot_.clear();
    std::ifstream fileIn(fileName);
    if (!fileIn)
        cout << "No hashdot file " << fileName;

    std::string line;
    while (std::getline(fileIn, line))
        hash_dot_.push_back(line);

    length_ = hash_dot_.back().length();
    width_ = hash_dot_.size();
}

string& HashDot::Random_Generate_String(std::string& line)
{
    char str[30] = { '.','.','.','.','#','.','.','.','.','.' ,'.','.','.','.','.' ,'.','.','.','.','.','.','.','.','.','.','.','.','.','.' };
    for (int i = 0; i < length_; i++)
        line.push_back(str[rand() % 30]);
    return line;
}


#endif