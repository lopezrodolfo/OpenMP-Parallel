/******************************************************************************
* FILE: omp_hello_spin.c
* DESCRIPTION:
*   OpenMP Example - Hello World - C/C++ Version
*   In this simple example, the primary thread forks a parallel region.
*   All threads in the team obtain their unique thread number and print it.
*   The primary thread only prints the total number of threads.  Two OpenMP
*   library routines are used to obtain the number of threads and each
*   thread's number.
* AUTHOR: Blaise Barney  5/99
* LAST REVISED: 04/06/05
*
* Modified 2023-03-19 Chuck Pateros, USD
* Randomizes the threads
* Waits using spin counters
* Includes several things for students to investigate/improve
* 
******************************************************************************/

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_WAIT_COUNT 4000000000   // 400000000000 or smaller   

int main (int argc, char *argv[]) 
{
int nthreads, tid;
int thread, initial_time;
int num_cores = omp_get_num_procs(); // varies by codespace 

unsigned long long int thread_wait_count_limit[num_cores];
unsigned long long int thread_wait_count[num_cores];
unsigned long long int my_thread_wait_count_limit;

// Next two lines will make runs more truly random.
initial_time = time(NULL);
srand(initial_time);

printf("OMP Detected %02d cores.\n\n",num_cores);

printf("MAX_WAIT_COUNT   %012lu\n\n", MAX_WAIT_COUNT);

for(thread=0;thread<num_cores;thread++){
  thread_wait_count_limit[thread]=(random() * random()) % MAX_WAIT_COUNT; 
	printf("Thread %02d  waits %012llu\n",thread,thread_wait_count_limit[thread]);
	}
printf("\n Beginning threads\n\n");
/* Fork a team of threads giving them their own copies of variables */
#pragma omp parallel private(nthreads, tid)
  {

  /* Obtain thread number */
  tid = omp_get_thread_num();

  /* wait a random amount of time */

  my_thread_wait_count_limit = thread_wait_count_limit[tid];

  // Simple spin wait counter
  for(thread_wait_count[tid]=0;thread_wait_count[tid]<my_thread_wait_count_limit; \
    thread_wait_count[tid]++);

  printf("Hello World from thread = %02d.  I waited %012llu\n",tid, thread_wait_count[tid]);

  /* Only primary thread will have this tid */
  if (tid == 0) // Good place for a breakpoint
    {
    nthreads = omp_get_num_threads();
    printf("      Number of threads = %02d.\n",nthreads);
    }

  } // end of #pragma

printf("All threads done.\n");
}  