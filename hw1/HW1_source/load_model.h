#ifndef LOAD_MODEL_H
#define LOAD_MODEL_H

#include <vector>
#include <string>

template <typename T>
std::vector<T> read_binary_file(const std::string& filename);

struct ModelWeights {
   std::vector<float> W0;  // 784 x 100
   std::vector<float> W1;  // 100 x 100
   std::vector<float> W2;  // 100 x 10
   std::vector<float> b0;  // 100
   std::vector<float> b1;  // 100
   std::vector<float> b2;  // 10
};

bool load_model_weights(const std::string& dir_path, ModelWeights& weights);

#endif // LOAD_MODEL_H 