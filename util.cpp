#include "util.h"

void log(string txt) {
  cerr << txt << endl;
}

pthread_t create_thread(void *start_routine, void *arg) {
  pthread_t thread;
  if (pthread_create(&thread, NULL, start_routine, arg) != 0)
    log("ERR! pthread_create!");
  return thread;
}
