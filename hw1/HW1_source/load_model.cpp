#include "load_model.h"
#include <iostream>
#include <fstream>
#include <filesystem>

template <typename T>
std::vector<T> read_binary_file(const std::string& filename) {
   std::ifstream file(filename, std::ios::binary);
   if (!file.is_open()) {
      throw std::runtime_error("Could not open file: " + filename);
   }
   
   // Get file size
   file.seekg(0, std::ios::end);
   std::streamsize size = file.tellg();
   file.seekg(0, std::ios::beg);
   
   // Read the data
   std::vector<T> buffer(size / sizeof(T));
   file.read(reinterpret_cast<char*>(buffer.data()), size);
   
   return buffer;
}
template std::vector<float> read_binary_file<float>(const std::string& filename);
template std::vector<int> read_binary_file<int>(const std::string& filename);

bool load_model_weights(const std::string& dir_path, ModelWeights& weights) {
   try {
      // Load weights
      weights.W0 = read_binary_file<float>(dir_path + "/W0.bin");
      weights.W1 = read_binary_file<float>(dir_path + "/W1.bin");
      weights.W2 = read_binary_file<float>(dir_path + "/W2.bin");
      
      // Load biases
      weights.b0 = read_binary_file<float>(dir_path + "/b0.bin");
      weights.b1 = read_binary_file<float>(dir_path + "/b1.bin");
      weights.b2 = read_binary_file<float>(dir_path + "/b2.bin");
      
      // Verify dimensions
      if (weights.W0.size() != 784 * 100 ||
          weights.W1.size() != 100 * 100 ||
          weights.W2.size() != 100 * 10 ||
          weights.b0.size() != 100 ||
          weights.b1.size() != 100 ||
          weights.b2.size() != 10) {
         throw std::runtime_error("Incorrect weight dimensions");
      }
      
      return true;
   } catch (const std::exception& e) {
      std::cerr << "Error loading weights: " << e.what() << std::endl;
      return false;
   }
} 