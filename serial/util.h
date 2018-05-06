#ifndef __UTILS__
#define __UTILS__

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <math.h>
using namespace std;

#define PI 3.14159265

void log(string txt);
vector<int> extract_int(string str, string delimiter);
vector<vector<int> > read_inputs();
vector<int> read_weights_and_bias() ;

#endif
