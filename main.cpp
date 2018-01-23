#include"Header.h"
int main()
{
    int    x = 0;
    int    y;
     //while (cin.get() != '0')
     //{
        Timer T;
        y = 0;
        const std::string fileName = "My_City.txt";
     // m.hash_dot.Random_Generate ();
        m.hash_dot.Read_From_File(fileName);
        m.Create_Vertexes();
        m.enumerateBuildings(); 
        m.Circle(); 
        m.print();
        cout << "-----------------------\nNumber of bridges.... " << count_of_bridges << endl;
        cout << "Total length......... "                          << bridge_length    << endl;
        cout << "Disconnected groups.. "                          << non_connected_buildings << "\n----------------------- \n";
        clean_all();
  //  }
    std::system("pause");
}



