#include"Header.h"

int main()
{
    string line;
    int    x = 0;
    int    y = 0;
    int a;

    while (cin >> a)
    {
        y = 0;
        cout << "City with hash dot\n";
        hash_dot.fileIn.seekg(0, ios::beg);
        hash_dot.Random_Generate();
        hash_dot.fileIn.seekg(0, ios::beg);

        while (getline(hash_dot.fileIn, line))
        {
            x = 0;
            for (char& c : line)
            {
                if (c == '#')
                    m.C4V(x, y);
                x++;
            }
            cout << '\n';
            y++;
        }

        m.enumerateBuildings();
        m.Circle();
        cout << "City with bridges is \n";
        m.print();
        cout << "Count of bridges is " << count_of_bridges << endl;

        hash_dot.fileIn.clear();

        //Clean all data structures.
        for (int j = 0; j < m.cols(); j++)
            for (int i = 0; i < m.rows(); i++)
                m(i, j) = nullptr;
        symmetric_corner_inners.clear();
        prev_bridge_directions.clear();
        prev_end = None;
        prev_start = None;
        count_of_bridges = 0;
        non_connected_buildings = 0;
    }

    std::system("pause");
}