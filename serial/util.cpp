#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <math.h>
using namespace std;

#define PI 3.14159265

vector<int> extract_int(string str, string delimiter) {
  vector<int> res;
  if (str.empty())
    return res;
  int pos;
  while ((pos = str.find(delimiter)) != -1) {
    string token = str.substr(0, pos);
    str.erase(0, pos + delimiter.length());
    res.push_back(atoi(token.c_str()));
  }
  res.push_back(atoi(str.c_str()));
  return res;
}

vector<vector<int> > read_inputs() {
  ifstream file("./inputs.txt");
  vector<vector<int> > res;
  if (file.is_open()) {
    string word;
    while (file >> word) {
      if (word.find(",") != -1) {
        word.erase(remove(word.begin(), word.end(), '{'), word.end());
        word.erase(remove(word.begin(), word.end(), '}'), word.end());
        res.push_back(extract_int(word, ","));
      }
    }
  }
  return res;
}

vector<int> read_weights_and_bias() {
  ifstream file("./weights.txt");
  vector<int> res;
  bool is_bias = false;
  if (file.is_open()) {
    string word;
    while (file >> word) {
      if (word.find(",") != -1) {
        word.erase(remove(word.begin(), word.end(), '{'), word.end());
        word.erase(remove(word.begin(), word.end(), '}'), word.end());
        res = extract_int(word, ",");
      }
      if (is_bias)
        res.push_back(atoi(word.c_str()));
      if (word.find("Bias") != -1)
        is_bias = true;
    }
  }
  return res;
}
