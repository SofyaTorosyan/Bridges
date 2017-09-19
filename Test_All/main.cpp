#include "../Header.h"

bool files_identical(std::string fileNameA, std::string fileNameB)
{
    std::ifstream A(fileNameA);
    std::ifstream B(fileNameB);

    std::stringstream a;
    std::stringstream b;

    a << A.rdbuf();
    b << B.rdbuf();

    return (a.str() == b.str());
}

void test_range(int a, int b)
{
    const std::string dirHDGold = "../Tests/Hashdots_gold/";
    const std::string dirConnected = "../Tests/Connected/";
    const std::string dirConnectedGold = "../Tests/Connected_gold/";

    for (int i = a; i < b; i++)
    {
        const std::string HDFile = dirHDGold + "model_hashdot_" + std::to_string(i) + ".txt";  // ??????
        const std::string CFile  = dirConnected + "model_connected_" + std::to_string(i) + ".txt";
        const std::string CGFile = dirConnectedGold + "model_connected_" + std::to_string(i) + ".txt";
        hash_dot.Read_From_File(HDFile); // Reads from ../Tests/Hashdots_gold/model_hash_dot_0 file...
        m.Create_Vertexes();
        m.enumerateBuildings();
        m.Circle();

        ofstream C_File;
        C_File.open(CFile);
        m.Write_To_File(C_File);

        if (!files_identical(CGFile, CFile))
            cout << "Not Identical";
        else
            cout << "OK";
        // m.print();
    }
}

int main()
{
    test_range(0, 2000);   
    std::system("pause");
}