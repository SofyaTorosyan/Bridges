#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

class HashDot
{
private:
    int            width_;
    int            length_;
    vector<string> hash_dot_;  
public:
    HashDot();
    HashDot(int, int);
    void clear();
    void print();
    int  width();
    int  length();
    int  row_size();
    int  col_size();
    char up         (int, int);
    char left       (int, int);   
    char upleft     (int, int);
    char& operator()(int, int); 
    void Random_Generate();
    void Read_From_File(const std::string);
    string& Random_Generate_String(std::string&);  
};