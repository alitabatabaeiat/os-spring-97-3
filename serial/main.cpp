#include "util.h"

int main(int argc, char const *argv[]) {
  vector<double> weights = read_weights_and_bias();
  double bias = weights.back();
  cout << bias << endl;
  weights.pop_back();
  vector<vector<double> > inputs = read_inputs();

  double sum;
  for (int i = 0; i < inputs.size(); i++) {
    sum = 0;
    for (int j = 0; j < inputs[i].size(); j++)
      sum += weights[j] * inputs[i][j];
    sum += bias;
    ofstream file("output.txt", ofstream::app);
    if (file.is_open())
      file << atan(sum) << endl;
  }
  return 0;
}
