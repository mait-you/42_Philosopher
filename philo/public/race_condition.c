#include <stdio.h>
#include <pthread.h>

int resource = 0;
pthread_mutex_t lock;

void *thread1(void *arg)
{
    pthread_mutex_lock(&lock);
    if (resource == 0) // ! race condition
	{
        printf("Thread 1 doing setup\n");
        resource = 1;
    }
    pthread_mutex_unlock(&lock);
    return (NULL);
}

void *thread2(void *arg)
{
    pthread_mutex_lock(&lock);
    if (resource == 0) // ! race condition
	{
        printf("Thread 2 proceeds too early!\n");
		resource = 1;
    }
    pthread_mutex_unlock(&lock);
    return (NULL);
}

int main()
{
    pthread_t t1, t2;
    pthread_mutex_init(&lock, (NULL));

    pthread_create(&t1, (NULL), thread1, (NULL));
    pthread_create(&t2, (NULL), thread2, (NULL));

    pthread_join(t1, (NULL));
    pthread_join(t2, (NULL));
    pthread_mutex_destroy(&lock);

    return (0);
}
