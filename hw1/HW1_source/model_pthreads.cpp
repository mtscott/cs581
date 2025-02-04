#include "model.h"
#include <cmath>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <chrono>
#include <omp.h>
#include <pthread.h>

#define NUM_THREADS 8

MNISTModel::MNISTModel(const ModelWeights& weights) : weights(weights) {}

// TODO: You can add your own helper functions here

struct thread_args_multiply {
    int start;
    int end;
    float *weight;
    int input_dim;
    int output_dim;
    int batch_size;
    float *output;
    float *input;
};

struct thread_args_bias {
    int start;
    int end;
    float *vector;
    float *bias;
};

struct thread_args_relu {
    int start;
    int end;
    float *vector;
};

void *add_bias(void *arg){
    struct thread_args_bias *range = (struct thread_args_bias *) arg;
    for (int i = range->start; i < range->end; i++) {
        range->vector[i] += range->bias[i % range->end];
    }
    return 0;
}

void *multiply(void *arg){
    struct thread_args_multiply *range = (struct thread_args_multiply *) arg;
    for (int i = range->start; i < range->end; i++) {
        for (int j = 0; j < range->output_dim; j++) {
            for (int k = 0; k < range->input_dim; k++) {
                range->output[i * range->output_dim + j] += range->input[i * range->input_dim + k] * range->weight[j * range->input_dim + k];
            }
        }
    }
    return 0;
}

int MNISTModel::linear_batch(const std::vector<float>& input, const std::vector<float>& weight, const std::vector<float>& bias, int input_dim, int output_dim, int batch_size, std::vector<float>& output) {

    // TODO: Your implementation here
    
    pthread_t child_threads_bias[NUM_THREADS];
    struct thread_args_bias work_ranges_bias[NUM_THREADS];
    int current_start, range;
    current_start = 0;
    range = output_dim / NUM_THREADS;
    for(int i = 0; i < NUM_THREADS; i++) {
        work_ranges_bias[i].start = current_start;
        work_ranges_bias[i].end = current_start + range;
        work_ranges_bias[i].vector = (float *) &output[0];
        work_ranges_bias[i].bias = (float *) &bias[0];
        current_start += range;
    }
    work_ranges_bias[NUM_THREADS-1].end = output_dim;

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&child_threads_bias[i], NULL, add_bias, &work_ranges_bias[i]);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(child_threads_bias[i], NULL);
    }

    // for(size_t i = 0; i < output.size(); i++) {
    //     // initialize output to 0 as a float
    //     output[i] = 0.0f;  
    //     output[i] = bias[i % output_dim];
    // }
    
    pthread_t child_threads[NUM_THREADS];
    struct thread_args_multiply work_ranges[NUM_THREADS];
    current_start = 0;
    range = batch_size / NUM_THREADS;
    for(int i = 0; i < NUM_THREADS; i++) {
        work_ranges[i].start = current_start;
        work_ranges[i].end = current_start + range;
        work_ranges[i].weight = (float *) &weight[0];
        work_ranges[i].input = (float *) &input[0];
        work_ranges[i].output = (float *) &output[0];
        work_ranges[i].input_dim = input_dim;
        work_ranges[i].output_dim = output_dim;
        work_ranges[i].batch_size = batch_size;
        current_start += range;
    }
    work_ranges[NUM_THREADS-1].end = batch_size;

    for(int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&child_threads[i], NULL, multiply, &work_ranges[i]);
    }
    for(int i = 0; i < NUM_THREADS; i++) {
        pthread_join(child_threads[i], NULL);
    }

    // for (int i = 0; i < batch_size; i++) {
    //     for (int j = 0; j < output_dim; j++) {
    //         for (int k = 0; k < input_dim; k++) {
    //             output[i * output_dim + j] += input[i * input_dim + k] * weight[j * input_dim + k];
    //         }
    //     }
    // }

   // TODO: End of your implementation

    return 0;
}

void *reLU(void *arg){ 
    struct thread_args_relu *range = (struct thread_args_relu *) arg;
    for (int i = range->start; i < range->end; i++) {
        if (range->vector[i] < 0.0f){
            range->vector[i] = 0.0f;
        } 
    }
    return 0;
}

int MNISTModel::relu_batch(std::vector<float>& vector) {

   // TODO: Your implementation here

    pthread_t child_threads[NUM_THREADS];
    struct thread_args_relu work_ranges[NUM_THREADS];
    int current_start, range;
    current_start = 0;
    range = vector.size() / NUM_THREADS;
    for(int i = 0; i < NUM_THREADS; i++) {
        work_ranges[i].start = current_start;
        work_ranges[i].end = current_start + range;
        work_ranges[i].vector = (float *) &vector[0];
        current_start += range;
    }
    work_ranges[NUM_THREADS-1].end = vector.size();


    for(int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&child_threads[i], NULL, reLU, &work_ranges[i]);
    }

    for(int i = 0; i < NUM_THREADS; i++) {
        pthread_join(child_threads[i], NULL);
    }

    // for (int i = 0; i < vector.size(); i++) {
    //     if (vector[i] < 0.0f){
    //         vector[i] = 0.0f;
    //     } 
    // }

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