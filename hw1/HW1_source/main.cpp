#include <iostream>
#include <fstream>
#include <omp.h>
#include <map>
#include "load_model.h"
#include "model.h"

// Simple argument parser
class ArgParser {
private:
   std::map<std::string, std::string> args;

public:
   ArgParser(int argc, char* argv[]) {
      for (int i = 1; i < argc; i += 2) {
         if (i + 1 < argc) {
            std::string key = argv[i];
            if (key[0] == '-') key = key.substr(1); // Remove leading '-'
            args[key] = argv[i + 1];
         }
      }
   }

   bool has(const std::string& key) const {
      return args.find(key) != args.end();
   }

   std::string get(const std::string& key, const std::string& default_value = "") const {
      auto it = args.find(key);
      return it != args.end() ? it->second : default_value;
   }

   int getInt(const std::string& key, int default_value = 0) const {
      if (!has(key)) return default_value;
      try {
         return std::stoi(get(key));
      } catch (...) {
         return default_value;
      }
   }
};

std::vector<float> read_test_data(const std::string& filename, int expected_size) {
   std::vector<float> data = read_binary_file<float>(filename);
   if (data.size() != expected_size) {
      throw std::runtime_error("Unexpected data size in " + filename);
   }
   return data;
}

std::vector<int> read_test_labels(const std::string& filename, int expected_size) {
   std::vector<int> data = read_binary_file<int>(filename);
   if (data.size() != expected_size) {
      throw std::runtime_error("Unexpected data size in " + filename);
   }
   return data;
}

int main(int argc, char* argv[]) {
   ArgParser args(argc, argv);
   const int max_test = 10000;
   
   // Parse arguments with defaults
   int num_test = args.getInt("ntests", max_test);
   int num_threads = args.getInt("nthreads", 1);

   std::cout << "Usage: " << argv[0] << " -ntests <1-" << max_test << "> -nthreads <num>" << std::endl;
   
   // Validate arguments
   if (num_test < 1 || num_test > max_test) {
      std::cerr << "Error: ntests must be between 1 and " << max_test << std::endl;
      return 1;
   }
   
   // Set number of threads
   omp_set_num_threads(num_threads);
   std::cout << "Testing with " << num_test << " test samples and " << num_threads << " threads" << std::endl;

   ModelWeights weights;
   if (!load_model_weights("weights", weights)) {
      return 1;
   }
   
   try {
      auto X_test = read_test_data("weights/X_test.bin", max_test * 784);
      auto y_test = read_test_labels("weights/y_test.bin", max_test);
      
      MNISTModel model(weights);
      auto predictions = model.predict_batch(X_test, num_test);
      
      int correct = 0;
      for (int i = 0; i < num_test; i++) {
         if (predictions[i] == static_cast<int>(y_test[i])) {
            correct++;
         }
      }
      
      float accuracy = 100.0f * correct / num_test;
      std::cout << "Test Accuracy: " << accuracy << "%" << std::endl;
      
   } catch (const std::exception& e) {
      std::cerr << "Error: " << e.what() << std::endl;
      return 1;
   }
   
   return 0;
} 