#ifndef __UTILS__
#define __UTILS__

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <semaphore.h>
#include <pthread.h>
#include <algorithm>
using namespace std;

#define PI 3.14159265

pthread_t create_thread(void *(*start_routine)(void *), void *arg);
vector<int> extract_int(string str, string delimiter);
vector<vector<int> > read_inputs();
vector<int> read_weights_and_bias();
#endif
