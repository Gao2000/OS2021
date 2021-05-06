#include <unistd.h>
#include <semaphore.h>

// #include <condition_variable>
#include <thread>
#include <mutex>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#define barber_use_time 200 // 200ms
sem_t customers;
sem_t barbers;
std::mutex w_mutex;

int waiting = 0, CHAIRS = 0, cus_num = 0;
int Serviced_client = 0, Unserviced_client = 0;
void barber();
void customer();

int main()
{
    int i;

    /* input file */
    std::ifstream in;
    std::string temp;
    in.open("input.txt", std::ios::in);
    if (!in)
        std::cout << "input.txt not exist" << std::endl;
    std::getline(in, temp); CHAIRS = std::stoi(temp);
    std::getline(in, temp); cus_num = std::stoi(temp);
    int time[cus_num] = { 0 };
    for (int j = 0; j < cus_num - 1; j++) {
        int cus_time;
        std::getline(in, temp); cus_time = std::stoi(temp);
        time[j] = cus_time;
    }

    std::thread barber_t(barber);

    for (int j = 0; j < cus_num; j++)
    {
        std::thread customer_t(customer);
        customer_t.detach();
        usleep(time[j] * 1000);
    }
    in.close();
    barber_t.join();

    std::cout << "Serviced clients: " << Serviced_client << std::endl;
    std::cout << "Unserviced_client: " << cus_num - Serviced_client << std::endl;
}

void barber()
{
    while ((Serviced_client + Unserviced_client) < cus_num)
    {
        sem_wait(&customers);
        w_mutex.lock();
        waiting--;
        w_mutex.unlock();
        usleep(barber_use_time*1000);
        sem_post(&barbers);
        std::cout << "Barber services a client, available seats: " << CHAIRS - waiting << std::endl;
        Serviced_client++;
    }
    return;
}

void customer()
{
    if (waiting < CHAIRS)
    {
        w_mutex.lock();
        waiting++;
        std::cout << "Client takes a seat, available seats: " << CHAIRS - waiting << std::endl;
        w_mutex.unlock();
        sem_post(&customers);
        sem_wait(&barbers);
    }
    else
    {
        Unserviced_client++;
    }
}