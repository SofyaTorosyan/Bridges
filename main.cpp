#include"Header.h"


int main()
{
    int    x = 0;
    int    y;
    while (cin.get() != '0')
    {
        Timer T;
        y = 0;
     
      //  hash_dot.Random_Generate ();
          hash_dot.Read_From_File  ();    
                m.Create_Vertexes   ();
              //  m.enumerateBuildings();
              //  m.Circle();
              //  m.print ();
        cout << "------------------------- \nCount of bridges is " << count_of_bridges<<"\n-------------------------" << endl;
        clean_all();
        cout << hash_dot.length() << " " << hash_dot.width() << endl;
    }
    std::system("pause");
}




/*
bool files_identical(std::string fileNameA, std::string fileNameB)
{}

int test_range(int a, int b)
{
    const std::string dirHDGold        = "../Tests/Hashdots_gold/";
    const std::string dirConnected     = "../Tests/Connected/";
    const std::string dirConnectedGold = "../Tests/Connected_gold/";

    for (int i = a; i < b; i++)
    {
        const std::string HDFile = dirHDGold + "model_hashdot_" + std::to_string(i) + ".txt";
        m.Create_Vertexes();
        m.enumerateBuildings();
        m.Circle();
       // m.print();


    }
}
*/
