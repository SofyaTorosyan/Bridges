#include "../Header.h"
#include<experimental\filesystem>
mutex mut;
string num_str(const std::string& fileNameA)
{
    std::ifstream A(fileNameA);
    if (!A)
        cout << "Not A";
    string str;
    string line;
    while (getline(A, line))
        for (char c : line)
            if (isdigit(c))
                str.push_back(c);
    return str; 
}


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
    const std::string dirHD_30_30        = "../Tests/Hashdots_30_30/";
    const std::string dirConnected       = "../Tests/Connected/";
    const std::string dirConnected_30_30 = "../Tests/Connected_30_30/";

    for (int i = a; i < b; i++)
    {
         std::string HDFile = dirHD_30_30 + "model_hashdot_" + std::to_string(i) + ".txt";

   
        if (!std::experimental::filesystem::exists(HDFile))
        {
            cout << '\a'<< i  << " No file  ----------------------------------------------------------------"<<endl;
            HDFile = dirHD_30_30 + "model_hashdot_" + std::to_string(++i) + ".txt";
        }
   
        const std::string B      = dirConnected + "model_connected_" + std::to_string(i) + ".txt";
        const std::string A      = dirConnected_30_30 + "model_connected_" + std::to_string(i) + ".txt";
  
      
       
        std::lock_guard<std::mutex> lock(mut); 
        cout << i << endl;
        hash_dot.Read_From_File(HDFile);
        m.Create_Vertexes();
        m.enumerateBuildings();
        m.Circle(); 

        m.Write_To_File(B); 
         
        if (num_str(A) != num_str(B))
        {
            cout << endl << " : Not Identical??????????????????????????????????????????????????????\n";
            cout << '\a';   
        }
        else
        {
            cout << endl <<i << " : OK\n";
          
        }
      
        clean_all();
    }  
}

int main()
{
    std::thread t1(test_range, 0,    1000 );
    std::thread t2(test_range, 1000,  2000);
    //std::thread t3(test_range, 1000, 1500);
    //std::thread t4(test_range, 1500, 2000);
  

  //  test_range(312, 1000);
    std::system("pause");
} 