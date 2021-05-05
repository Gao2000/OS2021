#include <thread>
#include <mutex>
#include <iostream>
#include <vector>
#include <cmath>
#include <condition_variable>

using namespace std;

int chairs = 10;
int empty_chairs = chairs; // 空椅子数
int customers = 0;
int barbers = 1;
condition_variable cv_barbers; // 当有顾客到时需通知理发师
mutex chairs_mtx, cus_mtx, bar_mtx;

/**
* 理发师进程，阻塞理发师的请况
* 1. 没有顾客，则睡觉（阻塞）
* 2. 访问临界区受阻，此时临界区正在被顾客访问
* @param i [description]
*/
void barber(int i)
{
    while (true)
    {
        unique_lock<mutex> lck(bar_mtx);
        cv_barbers.wait(lck, []
        {
            if (customers > 0)
            {
                cout << "有顾客，理发师被唤醒" << endl;
                return true;
            }
            else
            {
                cout << "没有顾客，理发师睡觉" << endl;
                return false;
            }
        });
        unique_lock<mutex> lck2(chairs_mtx);
        customers--;
        empty_chairs++;
        /* cut hair*/
        cout << "理发师给顾客理发" << endl;
        lck2.unlock();
        // 理发时不断有顾客进来
        this_thread::sleep_for(std::chrono::microseconds(10));
    }
}
/**
* 顾客进程，阻塞顾客进程的情况有两种
* 1. 访问临界区（检查是否有空闲的椅子）时发现理发师进程也在访问临界区，P(chairs_mtx)
* 没有多余的椅子并不是阻塞顾客进程，直至有空闲椅子，而是直接离开，即该顾客不排队理发
* @param i [description]
*/
void customer(int i)
{
    unique_lock<mutex> lck(chairs_mtx);
    if (empty_chairs > 0)
    {
        empty_chairs--;
        customers++;
        cv_barbers.notify_one();
        cout << "顾客 " << i << " 等待理发" << endl;
        //this_thread::sleep_for(std::chrono::milliseconds(100));
        lck.unlock();
    }
    else
    {
        /* 进程退出，不再理发了 */
        cout << "顾客 " << i << " 没有位置，离开" << endl;
        lck.unlock();
        /* leave */
    }
}

int main()
{
    thread t1 = thread(barber, 1);
    vector<thread> v;
    for (size_t i = 0; i < 20; i++)
    {
        v.push_back(thread(customer, i + 1));
    }
    t1.join();
    for (size_t i = 0; i < v.size(); i++)
    {
        v[i].join();
    }
    system("pause");
    return 0;
}
// ————————————————
// 版权声明：本文为CSDN博主「Cristiano - Duan」的原创文章，遵循CC 4.0 BY - SA版权协议，转载请附上原文出处链接及本声明。
// 原文链接：https ://blog.csdn.net/duanzhengbing/article/details/52141699