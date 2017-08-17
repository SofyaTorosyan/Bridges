#include"Header.h"

int  row_size();
int  col_size();

int main()
{
    string line;
    int    x = 0;
    int    y = 0;
    cout << m.rows();
    hashdot = new char*[m.rows() - 1];
    for (int i = 0; i < m.rows() - 1; i++)
        hashdot[i] = new char[m.cols() - 1];

    if (!file)
    {
        cout << "No file is found";
        return -1;
    }

    cout << "City with hash dot\n";
    file.seekg(0, ios::beg);
    while (getline(file, line))
    {
        x = 0;
        for (char& c : line)
        {
            if (c == '#')
            {
                cout << '#';
                hashdot[x][y] = '#';
                m.C4V(x, y);
            }
            else
            {
                cout << '.';
                hashdot[x][y] = '.';
            }
            x++;
        }
        cout << '\n';
        y++;
    }


    file.clear();
    cout << '\n';
   
    m.enumerateBuildings();
    m.Circle();
    cout << "City with bridges is \n";
    m.print();
    cout << "Count of bridges is " << count_of_bridges<<endl;
    cout << "Count of non connected bridges is " << --non_connected_buildings<<endl;
    std::system("pause");
}
  