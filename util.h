#include <semaphore.h>
#include <pthread.h>
using namespace std;

void log(string txt);
pthread_t create_thread(void *start_routine, void *arg);
