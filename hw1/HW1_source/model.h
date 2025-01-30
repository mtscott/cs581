#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include "load_model.h"

class MNISTModel {
public:
   MNISTModel(const ModelWeights& weights);
   
   // Forward pass for multiple samples
   int forward_batch(const std::vector<float>& input, int batch_size, std::vector<float>& output);
   
   // Get predictions for batch
   std::vector<int> predict_batch(const std::vector<float>& input, int batch_size);

private:
   static constexpr int INPUT_SIZE = 784;   // 28 * 28
   static constexpr int HIDDEN_SIZE = 100;
   static constexpr int OUTPUT_SIZE = 10;
   
   ModelWeights weights;

   int linear_batch(const std::vector<float>& input, const std::vector<float>& weight, const std::vector<float>& bias, int input_dim, int output_dim, int batch_size, std::vector<float>& output);
   int relu_batch(std::vector<float>& vector);
};

#endif // MODEL_H 