#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/time.h>
#include <math.h>

#include <thread>
#include <mutex>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#define barber_use_time 200000 // 200ms
sem_t customers;
sem_t barbers;
pthread_mutex_t mutex;

int waiting = 0, CHAIRS = 0, cus_num = 0;
int Serviced_client = 0, Unserviced_client = 0;
void* barber(void* arg);
void* customer(void* num);
void cut_hair(void);

int main()
{
    int i;
    pthread_t barber_t, customer_t;

    /* input file */
    std::ifstream in;
    std::string temp;
    in.open("input.txt", std::ios::in);
    if (!in)
        std::cout << "input.txt not exist" << std::endl;
    std::getline(in, temp); CHAIRS = std::stoi(temp);
    std::getline(in, temp); cus_num = std::stoi(temp);
    int time[cus_num] = {0};
    for (int j = 0; j < cus_num - 1; j++) {
        int cus_time;
        std::getline(in, temp); cus_time = std::stoi(temp);
        time[j] = cus_time;
    }

    int error;
    error = pthread_create(&barber_t, NULL, barber, NULL);
    if (error != 0) {
        printf("pthread_create is not created.../n");
        return -1;
    }

    for(int j = 0; j < cus_num; j++)
    {
        // std::cout << cus_time << std::endl;
        error = pthread_create(&customer_t, NULL, customer, NULL);
        if (error != 0) {
            printf("pthread_create is not created...\n");
            return -1;
        }
        usleep(time[j]*1000);
    }
    in.close();

    std::cout << "Serviced clients: " << Serviced_client << std::endl;
    std::cout << "Unserviced_client: " << cus_num - Serviced_client << std::endl;
}
void* barber(void* arg)
{
    while ( (Serviced_client + UnServiced_client) <= cus_num)
    {
        sem_wait(&customers);
        pthread_mutex_lock(&mutex);
        waiting--;
        pthread_mutex_unlock(&mutex);
        cut_hair();
        sem_post(&barbers);
        std::cout << "Barber services a client, available seats: " << CHAIRS - waiting << std::endl;
        Serviced_client++;
    }
}
void cut_hair(void)
{
    usleep(barber_use_time);
}
void* customer(void* num)
{
    if (waiting < CHAIRS)
    {
        pthread_mutex_lock(&mutex);
        waiting++;
        std::cout << "Client takes a seat, available seats: " << CHAIRS - waiting << std::endl;
        pthread_mutex_unlock(&mutex);
        sem_post(&customers);
        sem_wait(&barbers);
    }
    else
    {
        //printf("  Waiter is too much...\n");
        Unserviced_client++;
    }

}

// ————————————————
// 版权声明：本文为CSDN博主「Ginray」的原创文章，遵循CC 4.0 BY - SA版权协议，转载请附上原文出处链接及本声明。
// 原文链接：https ://blog.csdn.net/Ginray/article/details/53916299