#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include <mutex>
#include <future>
#include <utility>   /* std::pair */
#include <fstream>
#include <cctype>
#include <sstream>
#include <iostream>
#include <algorithm>

using namespace std;
class Timer
{
private:
    std::chrono::steady_clock::time_point start;
public:
    Timer()
    {
        cout << "Time counting was started \n";
        start = std::chrono::steady_clock::now();
    }
    ~Timer()
    {
        auto end = std::chrono::steady_clock::now();
        cout << '\a' << '\a' << '\a' << "\nTime  was " << std::chrono::duration_cast< std::chrono::seconds>(end - start).count() << " seconds\n";
    }
};
