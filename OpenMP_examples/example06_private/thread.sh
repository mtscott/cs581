#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $0 <number_of_threads>"
    exit 1
fi

export OMP_NUM_THREADS=$1
echo "OpenMP threads set to $OMP_NUM_THREADS"