#include"Header.h"

int main()
{
    string line;
    int    x = 0;
    int    y = 0;
    while (cin.get() != '0')
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
            y++;
        }
        cout << endl;
        m.enumerateBuildings();
        m.Circle();
        cout << "City with bridges is\n";
        m.print();
        cout << "------------------------- \nCount of bridges is " << count_of_bridges<<"\n-------------------------" << endl;
        hash_dot.fileIn.clear();
        clean_all();
    }
    std::system("pause");
}
