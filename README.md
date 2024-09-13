# OpenMP Parallel Programming

This repository contains two OpenMP-based C programs demonstrating parallel programming concepts.

## Authors

- CEO: Lucca Fabani
- PM: Rodolfo Lopez
- Dev0: Kera Hernandez
- Dev1: Nate Spinks
- Test: Michael Gallagher

## Date

4/3/23

## 1. omp_hello_spin.c

A modified "Hello World" program using OpenMP to demonstrate parallel execution with randomized thread waiting times.

### Features:

- Detects and utilizes available CPU cores
- Randomizes thread wait times
- Uses spin counters for waiting

### Compilation:

```
gcc -fopenmp omp_hello_spin.c -o omp_hello_spin
```

### Execution:

```
./omp_hello_spin
```

## 2. bodysurfers.c

A variant of the dining philosophers problem, implemented with bodysurfers sharing fins.

### Features:

- Simulates multiple surfers sharing a limited number of fins
- Uses OpenMP locks for synchronization
- Configurable number of surfers and waves

### Compilation:

```
gcc -fopenmp bodysurfers.c -o bodysurfers
```

### Execution:

```
./bodysurfers
```

## Requirements

- GCC compiler with OpenMP support
- POSIX-compliant system (for usleep function)

## Notes

- Both programs demonstrate different aspects of parallel programming and synchronization.
- Adjust constants in the source code to modify behavior (e.g., NUM_SURFERS, NUM_WAVES, wait times).

## Acknowledgements

- Blaise Barney wrote `omp_hello_spin.c`
- Professor Chuck Pateros modified `omp_hello_spin.c` and wrote `body_surfers.c`
- Our team modified `omp_hello_spin.c` and `body_surfers.c`
