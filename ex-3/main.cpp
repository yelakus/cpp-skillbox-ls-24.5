#include <iostream>
#include <thread>
#include <chrono> 
#include <ctime>
#include <iomanip>

int main(){
    int total_sec;
    std::time_t t = std::time(nullptr);
    std::tm timer = *std::localtime(&t);
    std::cout << "Enter minutes and seconds (mm:ss):" << std::endl;
    std::cin >> std::get_time(&timer, "%M:%S"); 

    total_sec = (timer.tm_min * 60) + timer.tm_sec;

    for (int i = total_sec; i > 0; --i) {

        std::cout << std::put_time(&timer, "%M:%S") << std::endl;

        if (timer.tm_sec == 0) 
        {
            timer.tm_sec = 60;
            timer.tm_min--;
        }

        timer.tm_sec--;
        std::this_thread::sleep_for(std::chrono::seconds(1)); 
    }

    std::cout << "DING! DING! DING!" << std::endl;

}