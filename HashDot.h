#pragma once
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include <mutex>
#include <future>
#include <utility>   /* std::pair */
#include <fstream>
#include <cctype>
#include <sstream>
#include <iostream>
#include <algorithm>

using namespace std;
class HashDot
{
private:
    int            width_;
    int            length_;
    vector<string> hash_dot_;
   
public:
    HashDot(int l, int w) : length_(l), width_(w)
    { }
    HashDot() : length_(30), width_(30)
    { }

    void clear()
    {
        hash_dot_.clear();
    }

    void print()
    {
        for (int i = 0; i < 30; i++)
            for (int j = 0; j < 30; j++)
                cout << hash_dot_[i][j];
        cout << endl;
    }
    string& Random_Generate_String(std::string& line)
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
            cout << line << endl;
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
        width_ = hash_dot_.size();
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

    char left(int x, int y)
    {
        if (x == 0)
            return '.';
        return hash_dot_[y][x - 1];
    }

    char up(int x, int y)
    {
        if (y == 0)
            return '.';
        return hash_dot_[y - 1][x];
    }

    char upleft(int x, int y)
    {
        if (y == 0 || x == 0)
            return '.';
        return hash_dot_[y - 1][x - 1];
    }
};