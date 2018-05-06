#include "thread_routine.h"

void* inputs_routine(void *) {
  cout << "inputs routine created with id " << pthread_self() << endl;
}

void* weights_routine(void *) {
  cout << "weights routine created with id " << pthread_self() << endl;
}

void* mid_routine(void *) {
  cout << "mid routine created with id " << pthread_self() << endl;
}

void* output_routine(void *) {
  cout << "output routine created with id " << pthread_self() << endl;
}
