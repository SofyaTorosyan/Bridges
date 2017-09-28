#include "../Header.h"

std::string strip_whitespaces(std::string s)
{
    s.erase(std::remove_if(s.begin(), s.end(), [](char c) { return std::isspace(c); }), s.end());
    return s;
}

bool files_identical(std::string fileNameA, std::string fileNameB)
{
    std::ifstream A(fileNameA);
    std::ifstream B(fileNameB); 

    if (!A)
        cout << "Not A";
    if (!B)
        cout << "Not B";
    
    std::stringstream a;
    std::stringstream b;

    a << A.rdbuf();
    b << B.rdbuf(); 

    string astr = a.str();  
    string bstr = b.str(); 

  //  auto pos = mismatch(astr.begin(), astr.end(), bstr.begin());
    return (strip_whitespaces(a.str()) == strip_whitespaces(b.str()));
} 
 

void test_range(int a, int b) 
{
    const std::string dirHD_10_10        = "../Tests/Hashdots_10_10/";
    const std::string dirConnected       = "../Tests/Connected/";
    const std::string dirConnected_10_10 = "../Tests/Connected_10_10/";

    for (int i = a; i < b; i++)
    {
        const std::string HDFile = dirHD_10_10 + "model_hashdot_" + std::to_string(i) + ".txt";  
        const std::string B  = dirConnected + "model_connected_" + std::to_string(i) + ".txt";
        const std::string A  = dirConnected_10_10 + "model_connected_" + std::to_string(i) + ".txt";
        hash_dot.Read_From_File(HDFile);
        m.Create_Vertexes();
        m.enumerateBuildings();
        m.Circle(); 

        m.Write_To_File(B); 

        if (!files_identical(A, B))
        {
            cout << "Not Identical";
            break;
        }
        else
            cout << "OK";
    } 
}

int main()
{
    test_range(5, 6);
    std::system("pause");
} 