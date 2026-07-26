#include "ip_boxer/timer.hpp"
#include <chrono>
#include <exception>
#include <functional>
#include "test.hpp"
#include <thread>
#include <tuple>
#include <vector>
using namespace std;

void test_running(){
    const char *test_name = "timer_test.test_running";
    printInfo(test_name);
    vector<tuple<int,int,int>> test_cases = {
        { 4, 500, 2100 },
        { 3, 100, 320 },
        { 9, 60, 560 }
    };

    for(auto test_case : test_cases) {
        try{
            int timer_delay = get<1>(test_case), total_delay = get<2>(test_case), expected_counter = get<0>(test_case);
            printInfo("timer_delay=%dms total_delay=%dms expected_result=%d", timer_delay, total_delay, expected_counter);
            timer t(timer_delay);
            int counter = 0;
            function<void()> event = [&](){
                counter++;
            };
            t.on_timeout(event);
            t.launch();

            this_thread::sleep_for(chrono::milliseconds(total_delay));
            if(counter != expected_counter)
                fail(test_name, "Expected %d, but got %d!", 1, expected_counter, counter);
        }
        catch(exception &e) {
            fail(test_name, e.what(), 2);
        }
        catch(...){
            fail(test_name, "Unknown exception!", 3);
        }
    }

    printPass(test_name);
}

void test_reset(){

}

int main(){
    test_running();
    return 0;
}