#include "thread_routine.h"
#include "util.h"

int main(int argc, char const *argv[]) {
  if (argc != 2) {
    cout << "ERR! Num of mid threads?" << endl;
    return -1;
  }

  int mid_threads_num = atoi((char *)argv[1]);

  // inputs thread
  pthread_t inputs_thread = create_thread(inputs_routine, NULL);
  if (inputs_thread == NULL) {
    cout << "ERR! inputs_thread creation error" << endl;
    return -1;
  }

  // weights thread
  pthread_t weights_thread = create_thread(weights_routine, NULL);
  if (weights_thread == NULL) {
    cout << "ERR! weights_thread creation error" << endl;
    return -1;
  }

  // middle threads
  pthread_t mid_thread[mid_threads_num];
  for (int i = 0; i < mid_threads_num; i++) {
    mid_thread[i] = create_thread(mid_routine, NULL);
    if (mid_thread[i] == NULL) {
      cout << "ERR! mid_thread[" << i << "] creation error" << endl;
      return -1;
    }
  }

  // output thread
  pthread_t output_thread = create_thread(output_routine, NULL);
  if (output_thread == NULL) {
    cout << "ERR! output_thread creation error" << endl;
    return -1;
  }
  cout << "-- All threads Created successfully!" << endl;

  if (pthread_join(inputs_thread, NULL) != 0)
    cerr << "ERR! Failed to join inputs thread" << endl;

  if (pthread_join(weights_thread, NULL) != 0)
    cerr << "ERR! Failed to join weights thread" << endl;

  for (int i = 0; i < mid_threads_num; i++)
    if (pthread_join(mid_thread[i], NULL) != 0)
      cout << "ERR! Failed to join middle thread index " << i << endl;

  if (pthread_join(output_thread, NULL) != 0)
    cerr << "ERR! Failed to join input thread" << endl;

  return 0;
}
