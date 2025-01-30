#include "model.h"
#include <cmath>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <chrono>
#include <omp.h>
#include <pthread.h>

MNISTModel::MNISTModel(const ModelWeights& weights) : weights(weights) {}

// TODO: You can add your own helper functions here

int MNISTModel::linear_batch(const std::vector<float>& input, const std::vector<float>& weight, const std::vector<float>& bias, int input_dim, int output_dim, int batch_size, std::vector<float>& output) {

   // TODO: Your implementation here

   for(size_t i = 0; i < output.size(); i++) {
      output[i] = 0.0f;  
   }

   // TODO: End of your implementation

   return 0;
}

int MNISTModel::relu_batch(std::vector<float>& vector) {

   // TODO: Your implementation here

   for (int i = 0; i < vector.size(); i++) {
      vector[i] = 0.0f;
   }

   // TODO: End of your implementation

   return 0;
}

int MNISTModel::forward_batch(const std::vector<float>& input, int batch_size, std::vector<float>& output) {
   if (input.size() < batch_size * INPUT_SIZE) {
      throw std::runtime_error("Invalid input size for batch");
   }
   if (output.size() != batch_size * OUTPUT_SIZE) {
      throw std::runtime_error("Invalid output size for batch");
   }
   
   std::vector<float> layer0_output(batch_size * HIDDEN_SIZE);
   std::vector<float> layer1_output(batch_size * HIDDEN_SIZE);

   linear_batch(input, weights.W0, weights.b0, INPUT_SIZE, HIDDEN_SIZE, batch_size, layer0_output);
   relu_batch(layer0_output);
   linear_batch(layer0_output, weights.W1, weights.b1, HIDDEN_SIZE, HIDDEN_SIZE, batch_size, layer1_output);
   relu_batch(layer1_output);
   linear_batch(layer1_output, weights.W2, weights.b2, HIDDEN_SIZE, OUTPUT_SIZE, batch_size, output);

   return 0;
}

std::vector<int> MNISTModel::predict_batch(const std::vector<float>& input, int batch_size) {

   std::vector<float> output(batch_size * OUTPUT_SIZE);

   auto start = std::chrono::high_resolution_clock::now();

   // This is the function you should work on
   forward_batch(input, batch_size, output);

   auto end = std::chrono::high_resolution_clock::now();
   auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
   std::cout << "Forward batch took " << duration << "ms" << std::endl;

   std::vector<int> predictions(batch_size);

   for (int i = 0; i < batch_size; i++) {
      predictions[i] = std::max_element(output.begin() + i * OUTPUT_SIZE, 
                                        output.begin() + (i + 1) * OUTPUT_SIZE) - 
                      output.begin() - i * OUTPUT_SIZE;
   }
   
   return predictions;
}