#include "util.h"

#define NUM_OF_INPUTS 128

sem_t console_sem;
sem_t bias_sem;
vector<sem_t> mid_sem_i;
vector<sem_t> mid_sem_w;
vector<sem_t> mid_sem_o;
int bias;
vector<vector<int> > weights;
vector<vector<int> > inputs;
vector<int> mid_val;
int mid_threads_num;

struct mid_thread_arg {
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

  mid_threads_num = atoi((char *)argv[1]);
  mid_val.resize(mid_threads_num, 0);

  sem_init(&console_sem, 0, 1);
  sem_init(&bias_sem, 0, 1);
  for (int i = 0; i < mid_threads_num; i++) {
    sem_t sem_i;
    sem_init(&sem_i, 0, 1);
    mid_sem_i.push_back(sem_i);

    sem_t sem_w;
    sem_init(&sem_w, 0, 1);
    mid_sem_w.push_back(sem_w);

    sem_t sem_o;
    sem_init(&sem_o, 0, 1);
    mid_sem_o.push_back(sem_o);
  }

  // inputs thread
  for (int i = 0; i < mid_threads_num; i++) {
    sem_wait(&mid_sem_i[i]);
    sem_wait(&mid_sem_o[i]);
  }
  pthread_t inputs_thread = create_thread(inputs_routine, NULL);
  if (inputs_thread == NULL) {
    cout << "ERR! inputs_thread creation error" << endl;
    return -1;
  }

  // weights thread
  sem_wait(&bias_sem);
  for (int i = 0; i < mid_threads_num; i++)
    sem_wait(&mid_sem_w[i]);
  pthread_t weights_thread = create_thread(weights_routine, NULL);
  if (weights_thread == NULL) {
    cout << "ERR! weights_thread creation error" << endl;
    return -1;
  }

  // middle threads
  pthread_t mid_thread[mid_threads_num];
  struct mid_thread_arg mid_thread_args[mid_threads_num];
  for (int i = 0; i < mid_threads_num; i++) {
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

  sem_wait(&console_sem);
  cout << "-- All threads Created successfully!" << endl;
  sem_post(&console_sem);

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
  sem_wait(&console_sem);
  cout << "inputs thread created with id " << pthread_self() << endl;
  sem_post(&console_sem);

  int share = NUM_OF_INPUTS / mid_threads_num;
  int r = NUM_OF_INPUTS % mid_threads_num;

  ifstream file("./inputs.txt");

  bool end = false;
  // while (!end) {
    bool start_r = false;
    int r_c = r;
    for (int i = 0; i < mid_threads_num; i++) {
      vector<int> input_row;
      string s;
      while(getline(file, s, ',')) {
        if (!start_r && s.find('{') != -1) {
          start_r = true;
          s.erase(0, s.find('{') + 1);
          s.erase(0, s.find('{') + 1);
          if (s.size() > 0)
            input_row.push_back(stoi(s));
        } else if (start_r && s.find('}') == -1) {
          input_row.push_back(stoi(s));
        } else if (start_r && s.find('}') != -1) {
          start_r = false;
          s.erase(s.find('}'));
          if (s.size() > 0)
            input_row.push_back(stoi(s));
        }
        if (r_c > 0 && input_row.size() == (share + 1)) {
          r_c--;
          break;
        } else if (r_c == 0 && input_row.size() == share) break;
      }
      inputs.push_back(input_row);
      sem_post(&mid_sem_i[i]);
    }
  // }
}

void* weights_routine(void *arg) {
  sem_wait(&console_sem);
  cout << "weights thread created with id " << pthread_self() << endl;
  sem_post(&console_sem);

  int share = NUM_OF_INPUTS / mid_threads_num;
  int r = NUM_OF_INPUTS % mid_threads_num;

  ifstream file("./weights.txt");

  bool start = false;
  for (int i = 0; i < mid_threads_num; i++) {
    vector<int> weight_row;
    string s;
    while(getline(file, s, ',')) {
      if (!start && s.find('{') != -1) {
        start = true;
        s.erase(0, s.find('{') + 1);
        if (s.size() > 0)
          weight_row.push_back(stoi(s));
      } else if (start && s.find('}') == -1) {
        weight_row.push_back(stoi(s));
      } else if (start && s.find('}') != -1) {
        start = false;
        string x = s.substr(0, s.find('}'));
        string b = s.substr(s.find("Bias"));
        b.erase(0, 6);
        bias = stoi(b);
        sem_post(&bias_sem);
        if (x.size() > 0)
          weight_row.push_back(stoi(x));
      }
      if (r > 0 && weight_row.size() == (share + 1)) {
        r--;
        break;
      } else if (r == 0 && weight_row.size() == share) break;
    }
    weights.push_back(weight_row);
    sem_post(&mid_sem_w[i]);
  }
}

void* mid_routine(void *arg) {
  int *id = (int *)arg;
  sem_wait(&console_sem);
  cout << "mid thread created with id " << *id << endl;
  sem_post(&console_sem);

  sem_wait(&mid_sem_i[*id]);
  sem_wait(&mid_sem_w[*id]);
  sem_wait(&console_sem);
  vector<int> w = weights[*id];
  vector<int> inp = inputs[*id];
  int sum = 0;
  for (int i = 0; i < w.size(); i++)
    sum += w[i] * inp[i];
  mid_val[*id] = sum;
  sem_post(&mid_sem_o[*id]);
  sem_post(&console_sem);

  // sem_wait(&weights_sem);
  // print(to_string(bias), &console_sem);
  // sem_post(&weights_sem);
}

void* output_routine(void *arg) {
  sem_wait(&console_sem);
  cout << "output thread created with id " << pthread_self() << endl;
  sem_post(&console_sem);

  int sum = 0;
  for (int i = 0; i < mid_threads_num; i++) {
    sem_wait(&mid_sem_o[i]);
    sum += mid_val[i];
  }
  sem_wait(&bias_sem);
  sum += bias;
  sem_post(&bias_sem);
  sem_wait(&console_sem);
  cout << sum << endl;
  cout << atan(sum) * 180.0 / PI << endl;
  sem_post(&console_sem);
}
