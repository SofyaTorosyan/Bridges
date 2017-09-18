#include "../Header.h"

int main()
{
    int    x = 0;
    int    y;
    while (cin.get() != '0')
    {
        Timer T;
        y = 0;

    //  hash_dot.Random_Generate ();
        hash_dot.Read_From_File();
       //m.Create_Vertexes();
       //m.enumerateBuildings();
       //m.Circle();
       //m.print ();
       //cout << "------------------------- \nCount of bridges is " << count_of_bridges << "\n-------------------------" << endl;
       //clean_all();
        cout << hash_dot.length() << " " << hash_dot.width() << endl;
    }
    std::system("pause");
}