#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/time.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#define barber_use_time 200000 // 200ms
sem_t customers;
sem_t barbers;
pthread_mutex_t mutex;

int waiting = 0;
int CHAIRS = 0;
int Serviced_client = 0;
void* barber(void* arg);
void* customer(void* num);
void cut_hair(void);
double flat(void);
double normal(void);
double bursty(void);

int main()
{
    int i;
    pthread_t barber_t, customer_t;

    /* input file */
    std::ifstream in;
    std::string temp;
    int cus_num = 0;
    in.open("input.txt", std::ios::in);
    if (!in)
        std::cout << "input.txt not exist" << std::endl;
    std::getline(in, temp); CHAIRS = std::stoi(temp);
    std::getline(in, temp); cus_num = std::stoi(temp);
    // printf("%d %d\n", cus_num, CHAIRS);
    int error;
    error = pthread_create(&barber_t, NULL, barber, NULL);
    if (error != 0) {
        printf("pthread_create is not created.../n");
        return -1;
    }

    int time[cus_num] = {0};
    for (int j = 1; j < cus_num; j++) {
        int cus_time;
        std::getline(in, temp); cus_time = std::stoi(temp);
        time[j] = cus_time;
    }

    error = pthread_create(&customer_t, NULL, customer, NULL);
    if (error != 0) {
        printf("pthread_create is not created...\n");
        return -1;
    }
    for(int j = 1; j < cus_num; j++)
    {
        // std::cout << cus_time << std::endl;
        usleep(time[j]*1000);
        error = pthread_create(&customer_t, NULL, customer, NULL);
        if (error != 0) {
            printf("pthread_create is not created...\n");
            return -1;
        }
    }
    in.close();

    std::cout << "Serviced clients: " << Serviced_client << std::endl;
    std::cout << "Unserviced_client: " << cus_num - Serviced_client << std::endl;
}
void* barber(void* arg)
{
    while (1)
    {
        sem_wait(&customers);
        pthread_mutex_lock(&mutex);
        waiting = waiting - 1;
        sem_post(&barbers);
        pthread_mutex_unlock(&mutex);
        usleep(barber_use_time);
        // cut_hair();
        std::cout << "Barber services a client, available seats: " << CHAIRS - waiting << std::endl;
        Serviced_client++;
    }
}
void cut_hair(void)
{
    // printf("  Barber:I am cutting the customer's hair...\n");
    usleep(barber_use_time);
    // printf("  Barber:done.\n");
}
void* customer(void* num)
{
    pthread_mutex_lock(&mutex);
    if (waiting < CHAIRS)
    {
        waiting = waiting + 1;
        sem_post(&customers);
        pthread_mutex_unlock(&mutex);
        sem_wait(&barbers);
        std::cout << "Client takes a seat, available seats: " << CHAIRS-waiting-1 << std::endl;
    }
    else
    {
        //printf("  Waiter is too much...\n");
        pthread_mutex_unlock(&mutex);
    }

}

// ————————————————
// 版权声明：本文为CSDN博主「Ginray」的原创文章，遵循CC 4.0 BY - SA版权协议，转载请附上原文出处链接及本声明。
// 原文链接：https ://blog.csdn.net/Ginray/article/details/53916299