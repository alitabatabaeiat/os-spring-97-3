#include "util.h"

sem_t console_sem;
sem_t inputs_sem;
sem_t weights_sem;
int bias;
vector<int> weights;
vector<vector<int> > inputs;
int ptr;

struct mid_thread_arg {
  int ptr;
  int share;
  int id;
};

void *inputs_routine(void *);
void *weights_routine(void *);
void *mid_routine(void *);
void *output_routine(void *);

int main(int argc, char const *argv[]) {
  if (argc != 2) {
    cout << "ERR! Num of mid threads?" << endl;
    return -1;
  }
  // ifstream file("./inputs.txt");
  // string s;
  // while(getline(file, s, ','))
  //   cout << s << endl;

  int mid_threads_num = atoi((char *)argv[1]);
  ptr = 0;

  if (sem_init(&console_sem, 0, 1) != 0)
    cout << "ERR! semaphore init" << endl;

  if (sem_init(&inputs_sem, 0, 1) != 0)
    cout << "ERR! semaphore init" << endl;

  if (sem_init(&weights_sem, 0, 1) != 0)
    cout << "ERR! semaphore init" << endl;
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

  int share = 128 / mid_threads_num;
  int r = 128 % mid_threads_num;
  print(to_string(share), &console_sem);
  print(to_string(r), &console_sem);

  // middle threads
  pthread_t mid_thread[mid_threads_num];
  struct mid_thread_arg mid_thread_args[mid_threads_num];
  for (int i = 0; i < mid_threads_num; i++) {
    mid_thread_args[i].ptr = ptr;
    if (r > 0) {
      mid_thread_args[i].share = share + 1;
      ptr += 1;
      r--;
    }
    else
      mid_thread_args[i].share = share;
    ptr += share;
    mid_thread_args[i].id = i;
    mid_thread[i] = create_thread(mid_routine, (void *)&mid_thread_args[i]);
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

  print("-- All threads Created successfully!", &console_sem);

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

void* inputs_routine(void *arg) {
  ostringstream oss;
  oss << "inputs thread created with id " << pthread_self();
  string txt = oss.str();
  print(txt, &console_sem);

  sem_wait(&inputs_sem);
  inputs = read_inputs();
  sem_post(&inputs_sem);
}

void* weights_routine(void *arg) {
  ostringstream oss;
  oss << "weights thread created with id " << pthread_self();
  string txt = oss.str();
  print(txt, &console_sem);

  sem_wait(&weights_sem);
  weights = read_weights_and_bias();
  bias = weights.back();
  weights.pop_back();
  sem_post(&weights_sem);
}

void* mid_routine(void *arg) {
  struct mid_thread_arg *mta = (struct mid_thread_arg *)arg;
  ostringstream oss;
  oss << "mid thread created with id " << mta->id << " - share = " << mta->share;
  string txt = oss.str();
  print(txt, &console_sem);

  sem_wait(&weights_sem);
  print(to_string(bias), &console_sem);
  sem_post(&weights_sem);
}

void* output_routine(void *arg) {
  ostringstream oss;
  oss << "output thread created with id " << pthread_self();
  string txt = oss.str();
  print(txt, &console_sem);
}
