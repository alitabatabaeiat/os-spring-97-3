#include <semaphore.h>
#include <pthread.h>
using namespace std;

#define PI 3.14159265


void log(string txt);
pthread_t create_thread(void *start_routine, void *arg);
vector<int> extract_int(string str, string delimiter);
vector<vector<int> > read_inputs();
vector<int> read_weights_and_bias();
