# Homework Assignment 1

In this homework assignment, we will be implementing a simple neural network to classify handwritten digits.

## Model Description

The model we use is a simple MLP with 2 hidden layers. The input layer has 784 neurons, the first hidden layer has 100 neurons, the second hidden layer has 100 neurons, and the output layer has 10 neurons.

1. Linear Layer 0:
   1. Weight Matrix: 784 x 100
   2. Bias Vector: 100
   3. Activation Function: ReLU
2. Linear Layer 1:
   1. Weight Matrix: 100 x 100
   2. Bias Vector: 100
   3. Activation Function: ReLU
3. Linear Layer 2:
   1. Weight Matrix: 100 x 10
   2. Bias Vector: 10

The weights and biases are provided to you in the `weights` folder, together with the test data and labels.

I have provided you with a simple starter code. In this code I have implemented most of the functions you need to implement, together with an incomplete implementation of the forward pass.

To get started:

1. Use the command `make` to compile the code.
2. Use the command `./foo.ex -ntests <number> -nthreads <number>` to run the code.
   1. ntests is the number of test samples you want to use, from 1 to 10000. You can start with a small number to verify the correctness of your implementation.
   2. nthreads is the number of threads you want to use.
   3. Both arguments are optional, and the default values are 10000 and 1. You can simply run `./foo.ex` to use the default values.
3. You should see a prediction accuracy of 9.8% since we have not implemented the forward pass yet.

## Tasks

Your tasks are as follows:

1. Finish the sequential implementation of the linear layer in the `linear_batch` function
   1. The input is a vector of size `input_dim * batch_size`
      - The i-th batch element is stored contiguously in memory, starting at `input[i * input_dim]`
   2. The weight matrix is of size `input_dim * output_dim`
      - The i-th row of the weight matrix is stored contiguously in memory, starting at `weight[i * input_dim]`
   3. The bias vector is of size `output_dim`
   4. The output is a vector of size `output_dim * batch_size`
      - The i-th batch element is stored contiguously in memory, starting at `output[i * output_dim]`
   5. Your implementation should be cache efficient in order to get full score.

2. Finish the sequential implementation of the ReLU activation function in the `relu_batch` function
   1. The input is a vector, you can use `vector.size()` to get the length of the vector
   2. The output is the same vector

3. Test the correctness of your implementation by running `./foo.ex -ntests 10000 -nthreads 1`
   1. You should have a prediction accuracy of about 98.14% with 10000 test samples

4. Use OpenMP to parallelize the forward pass.
   1. Both `linear_batch` and `relu_batch` should be parallelized.
   2. After finishing the implementation, backup your `model.cpp` as `model_openmp.cpp`

5. Use Pthreads to parallelize the forward pass.
   1. Both `linear_batch` and `relu_batch` should be parallelized.
   2. After finishing the implementation, backup your `model.cpp` as `model_pthreads.cpp`

## Submission

Please submit the following files in a single zip file:
1. `model_openmp.cpp`

Your OpenMP version of the code.

2. `model_pthreads.cpp`

Your Pthreads version of the code.

3. A pdf report of your implementation with the following sections:
   1. The environment you used to run the code (Operating System, CPU, RAM, etc.)
   2. The performance of your sequential implementation with all 10000 test samples
      1. The prediction accuracy
      2. The running time. Repeat 5 times, remove the longest and shortest time, and take the average of the remaining 3 times.
   3. The performance of your OpenMP implementation with all 10000 test samples
      1. The prediction accuracy
      2. The running time. Run your code with 2, 4, and 8 threads
         1. For each thread number, repeat 5 times, remove the longest and shortest time, and take the average of the remaining 3 times.
         2. Compute the speedup compared to the sequential implementation.
         3. Plot the speedup as a function of the number of threads.
   4. The performance of your Pthreads implementation with all 10000 test samples
      1. The prediction accuracy
      2. The running time. Run your code with 2, 4, and 8 threads
         1. For each thread number, repeat 5 times, remove the longest and shortest time, and take the average of the remaining 3 times.
         2. Compute the speedup compared to the sequential implementation.
         3. Plot the speedup as a function of the number of threads.
   5. Briefly discuss your implementation.
      1. How do you design your sequential implementation to be cache efficient?
      2. How do you parallelize the linear layer and the ReLU activation function?
      3. What is the performance bottleneck of your parallel implementation?

## Notes:

1. You can use LLMs to help you writing your code, but you should understand the code you write.
2. You cannot use LLMs to write your report.
3. Your report can be simple, it is not designed to take a lot of time. There are no unique correct answer for the discussion part so just write your thoughts.
4. Your don't need to get perfect speedup to get full score.
   1. Just to make sure when running with 2 threads you get over 1.5x speedup.
   2. And make sure that the output is correct.

