#include <iostream>
#include <future>
#include <thread>

using std::cout, std::endl;

std::future<void> TestAsync()
{
    cout << "TestAsync before\tThreadId: " << std::this_thread::get_id() << endl; // mainと同じスレッドID
    auto f = std::async(std::launch::async,
                        []()
                        {
                            cout << "std::async start" << endl;
                            cout << "std::async\tThreadId: " << std::this_thread::get_id() << endl;
                            std::this_thread::sleep_for(std::chrono::seconds(3));
                            cout << "std::async finish" << endl;
                        });
    cout << "TestAsync after\tThreadId: " << std::this_thread::get_id() << endl; // mainと同じスレッドID
    return {};
}

int main()
{
    cout << "main before \tThreadId: " << std::this_thread::get_id() << endl;
    auto t = TestAsync();
    cout << "main after \tThreadId: " << std::this_thread::get_id() << endl; // mainと同じスレッドID

    cout << "\n";

    cout << "wait start1" << endl;
    auto start1 = std::chrono::system_clock::now();
    auto f1 = std::async(std::launch::async,
                         []() { std::this_thread::sleep_for(std::chrono::seconds(3)); });
    auto f2 = std::async(std::launch::async,
                         []() { std::this_thread::sleep_for(std::chrono::seconds(3)); });
    f1.wait();
    f2.wait();
    auto end1 = std::chrono::system_clock::now();
    cout << "time1: " << std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1).count() << "ms" << endl;

    cout << "\n";

    cout << "wait start2" << endl;
    auto start2 = std::chrono::system_clock::now();
    {
        // futureのデストラクタでwaitされるのでf1, f2のように明示的にwaitしなくてよい
        auto f4 = std::async(std::launch::async,
                             []() { std::this_thread::sleep_for(std::chrono::seconds(3)); });
        auto f3 = std::async(std::launch::async,
                             []() { std::this_thread::sleep_for(std::chrono::seconds(3)); });
    }
    auto end2 = std::chrono::system_clock::now();
    cout << "time2: " << std::chrono::duration_cast<std::chrono::milliseconds>(end2 - start2).count() << "ms" << endl;

    cout << "\n";

    cout << "wait start3" << endl;
    auto start3 = std::chrono::system_clock::now();
    // futureを変数で受けないと、その場でwaitされる (6秒かかる)
    std::async(std::launch::async,
               []() { std::this_thread::sleep_for(std::chrono::seconds(3)); });
    std::async(std::launch::async,
               []() { std::this_thread::sleep_for(std::chrono::seconds(3)); });
    auto end3 = std::chrono::system_clock::now();
    cout << "time3: " << std::chrono::duration_cast<std::chrono::milliseconds>(end3 - start3).count() << "ms" << endl;

    cout << "\n";

    cout << "wait start4" << endl;
    auto start4 = std::chrono::system_clock::now();
    // futureを変数で受けないと、その場でwaitされる (6秒かかる)
    {
        // ブロックで囲っていても同様
        std::async(std::launch::async,
                   []() { std::this_thread::sleep_for(std::chrono::seconds(3)); });
        std::async(std::launch::async,
                   []() { std::this_thread::sleep_for(std::chrono::seconds(3)); });
    }
    auto end4 = std::chrono::system_clock::now();
    cout << "time4: " << std::chrono::duration_cast<std::chrono::milliseconds>(end4 - start4).count() << "ms" << endl;
}
