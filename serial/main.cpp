#include "util.cpp"

int main(int argc, char const *argv[]) {
  vector<int> weights = read_weights_and_bias();
  int bias = weights.back();
  weights.pop_back();
  vector<vector<int> > inputs = read_inputs();

  int sum;
  for (int i = 0; i < inputs.size(); i++) {
    sum = 0;
    for (int j = 0; j < inputs[i].size(); j++)
      sum += weights[j] * inputs[i][j];
    sum += bias;
    double row_res = atan(sum) * 180 / PI;
    ofstream file("output.txt", ofstream::app);
    if (file.is_open())
      file << row_res << endl;
  }
  return 0;
}
