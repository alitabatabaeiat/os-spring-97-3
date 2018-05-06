#include "thread_routine.h"

void* inputs_routine(void *arg) {
  sem_t *sem = (sem_t *)arg;
  if (sem_wait(sem) != 0)
    cout << "ERR! sem wait" << endl;
  cout << "inputs routine created with id " << pthread_self() << endl;
  if (sem_post(sem) != 0)
    cout << "ERR! sem post" << endl;
}

void* weights_routine(void *arg) {
  sem_t *sem = (sem_t *)arg;
  if (sem_wait(sem) != 0)
    cout << "ERR! sem wait" << endl;
  cout << "weights routine created with id " << pthread_self() << endl;
  if (sem_post(sem) != 0)
    cout << "ERR! sem post" << endl;
}

void* mid_routine(void *arg) {
  sem_t *sem = (sem_t *)arg;
  if (sem_wait(sem) != 0)
    cout << "ERR! sem wait" << endl;
  cout << "mid routine created with id " << pthread_self() << endl;
  if (sem_post(sem) != 0)
    cout << "ERR! sem post" << endl;
}

void* output_routine(void *arg) {
  sem_t *sem = (sem_t *)arg;
  if (sem_wait(sem) != 0)
    cout << "ERR! sem wait" << endl;
  cout << "output routine created with id " << pthread_self() << endl;
  if (sem_post(sem) != 0)
    cout << "ERR! sem post" << endl;
}
