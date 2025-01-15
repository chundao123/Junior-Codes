#include<iostream>
#include<pthread.h>
#include<semaphore.h>
#include<mutex>
#include<unistd.h>

using namespace std;

#define PH_NUM 5
#define FORK_NUM 5
#define PASTA_NUM 25

int ids[PH_NUM];
int result[PH_NUM] = { 0 };
int pasta_num = PASTA_NUM;
mutex mtx;
sem_t room;
sem_t forks[FORK_NUM];

void* TryEat(void* n)
{
    int id = ids[*(int*)n];
    while(pasta_num > 0)
    {
        //think
        //cout << "PH" << id << " is thinking\n";
        sleep(0.3);

        //wait for place and forks
        //cout << "PH" << id << " is hungry\n";
        sem_wait(&room);
        sem_wait(&forks[id]);
        sem_wait(&forks[(id+1) % FORK_NUM]);

        //enioy pasta
        mtx.lock();
        if(pasta_num > 0)
        {
            //eat
            sleep(0.1);
            cout << "PH" << id << " is eating the " << ++result[id] << " time.";
            cout << " Pasta left " << --pasta_num << "\n";
        }
        mtx.unlock();

        //leave
        sem_post(&forks[(id+1) % FORK_NUM]);
        sem_post(&forks[id]);
        sem_post(&room);
    }
    
    return NULL;
}

int main(int argc, char **argv)
{
    //read num of pasta
    if(argc > 1)
        pasta_num = atoi(argv[1]);
    
    //init setting
    sem_init(&room, 0, 2);
    for(int i = 0; i < FORK_NUM; i++)
        sem_init(&forks[i], 0, 1);

    //create thread
    pthread_t philosophers[PH_NUM];
    for(int i = 0; i < PH_NUM; i++)
    {
        ids[i] = i;
        pthread_create(&philosophers[i], NULL, TryEat, &ids[i]);
    }
    for(int i = 0; i < PH_NUM; i++)
        pthread_join(philosophers[i], NULL);

    //clean
    sem_destroy(&room);
    for(int i = 0; i < FORK_NUM; i++)
        sem_destroy(&forks[i]);
    
    //output
    for(int i = 0; i < PH_NUM; i++)
        cout << "PH" << i << " ate " << result[i] << " pasta\n";

    return 0;
}