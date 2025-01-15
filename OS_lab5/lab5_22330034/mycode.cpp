#include<semaphore.h>
#include<thread>
#include<iostream>

using namespace std;

sem_t rmutex, wmutex, S;
int readcount = 0, writecount = 0;

void read(string user, int i)
{
    cout << user << " read for " << i << " time\n";
}

void write(string user, int i)
{
    cout << user << " write for " << i << " time\n";
}

void Reader(string user, int trytime)
{
    for (int i = 0; i < trytime; i++)
    {
        sem_wait(&S);
        sem_wait(&rmutex);
        if(!readcount)
            sem_wait(&wmutex);
        readcount++;
        sem_post(&rmutex);
        sem_post(&S);

        read(user, i+1);

        sem_wait(&rmutex);
        readcount--;
        if(!readcount)
            sem_post(&wmutex);
        sem_post(&rmutex);
    }
    
}

void Writer(string user, int trytime)
{
    for (int i = 0; i < trytime; i++)
    {
        sem_wait(&S);
        sem_wait(&wmutex);

        write(user, i+1);

        sem_post(&wmutex);
        sem_post(&S);
    } 
}

int main()
{
    sem_init(&rmutex, 0, 1);
    sem_init(&wmutex, 0, 1);
    sem_init(&S, 0, 1);

    pthread_t tid;
    thread r1(Reader, "Hu", 5);
    thread r2(Reader, "Dao", 5);
    thread w1(Writer, "Hai", 5);
    thread r3(Reader, "Chun", 5);

    r1.join();
    r2.join();
    w1.join();
    r3.join();

    return 0;
}
