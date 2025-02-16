#pragma once

// Please do not modify this file.

#include <iostream>
#include "mpi.h"

float* parallel_quicksort(float* data, int length, MPI_Comm comm);