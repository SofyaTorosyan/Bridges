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
        clean_all();
       
    }

    std::system("pause");
}
