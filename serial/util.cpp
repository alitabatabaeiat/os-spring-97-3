#include "util.h"

vector<vector<double> > read_inputs() {
  ifstream file("./inputs.txt");
  vector<vector<double> > res;
  if (file.is_open()) {
    string word;
    while (file >> word) {
      if (word.find(",") != -1) {
        word.erase(remove(word.begin(), word.end(), '{'), word.end());
        word.erase(remove(word.begin(), word.end(), '}'), word.end());
        word.erase(word.size()-1, 1);
        res.push_back(extract_int(word, ","));
      }
    }
  }
  return res;
}

vector<double> read_weights_and_bias() {
  ifstream file("./weights.txt");
  vector<double> res;
  bool is_bias = false;
  if (file.is_open()) {
    string word;
    while (file >> word) {
      if (word.find(",") != -1) {
        word.erase(remove(word.begin(), word.end(), '{'), word.end());
        word.erase(remove(word.begin(), word.end(), '}'), word.end());
        word.erase(0, word.find("weights") + 8);
        res = extract_int(word, ",");
      }
      if (is_bias)
        res.push_back(stod(word));
      if (word.find("bias") != -1)
        is_bias = true;
    }
  }
  return res;
}

vector<double> extract_int(string str, string delimiter) {
  vector<double> res;
  if (str.empty())
    return res;
  int pos;
  while ((pos = str.find(delimiter)) != -1) {
    string token = str.substr(0, pos);
    str.erase(0, pos + delimiter.length());
    res.push_back(stod(token));
  }
  res.push_back(stod(str));
  return res;
}
