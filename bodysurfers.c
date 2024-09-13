/* bodysurfers.c
 * A variant of the dining philosophers problem using bodysurfers.
 *
 *
 * Each bodysurfer brings one fin.  They each put their fin into
 * a shared locker.
 * If at least two fins are available, they surf.
 *
 * Chuck Pateros, University of San Diego
 * 14-Mar-2021
 * 
 * 2023-03-27
 * Added analysis tools (getcpu, extra print statements, etc.)
 *
 * derived from http://monismith.info/cs599/examples/ompPhilosophers.c
 */

#include <omp.h>        // Use OpenMP
#include <stdio.h>      // Provide I/O
#include <unistd.h>     // Use usleep (sleep for a number of microseconds)
#include <time.h>       // Document run time
#include <sched.h>      // Access to scheduler functions

int sched_getcpu(void);

#define NUM_SURFERS 5     //The number of bodysurfers
#define NUM_WAVES 50000   //The number of waves per bodysurfer

//Number of microseconds to wait.  Defaults are 50 or 0
#define AFTER_SURF_WAIT 50 
#define AFTER_CHECK_WAIT 50

omp_lock_t fin_locker_lock; //Lock to represent the fin locker lock
int fin_locker_count; // the number of fins in the locker
int my_fin_locker_count;  //local count for debugging


void surfer()
{

  //Wait for all threads to start
  #pragma omp barrier

  //Set up variables for the thread id and fins.
  int surfer_id = omp_get_thread_num();
  int my_fin_count = 0;
  int my_fin_locker_count; //local variable to follow global
  int cpu_num = sched_getcpu();
  printf("Surfer %3d is running on CPU %3d\n", surfer_id, cpu_num);


  // We will count our waves to determine when we're too tired 
  int wave_count;
  for(wave_count = 0; wave_count < NUM_WAVES; wave_count++)
  {
    my_fin_count = 0;
    while(my_fin_count < 2)
    {
      omp_set_lock(&fin_locker_lock); //Wait for access to the fin locker

      if(fin_locker_count >= 2)
      {
        fin_locker_count -= 2; // remove two fins
        my_fin_locker_count = fin_locker_count; // local copy for debugging
        my_fin_count = 2;
      }
      
      omp_unset_lock(&fin_locker_lock); // make locker available again

      #if AFTER_CHECK_WAIT > 0
        usleep(AFTER_CHECK_WAIT); // have a refreshing beverage
      #endif

    } //end while waiting for 2 fins

// Cowabunga

    // comment out these two lines for short runs
    //  if((wave_count > 0)&&(wave_count%1000)==0) 
	  //     printf("phil %d is surfing wave %20d\r", surfer_id,wave_count);

    // comment out this line for long runs
    printf("phil %d is surfing wave %20d\n", surfer_id,wave_count); // print every time

    // End debugging print section


    usleep(100); // use this line to simulate surfing time

    omp_set_lock(&fin_locker_lock); //Wait for access to the fin locker
 
    fin_locker_count -= 2; // return fins
    my_fin_locker_count = fin_locker_count; // local copy for debugging
    my_fin_count = 0;
    
    omp_unset_lock(&fin_locker_lock);

    #if AFTER_SURF_WAIT > 0
      usleep(50); // after surf rest is important
    #endif

  } //end wave count
  printf("\n                      phil %d exhausted \n",surfer_id);
} // end surfer

int main(int argc, char ** argv)
{


  double elapsed_time;
  time_t curtime;

  time(&curtime);

  printf("OMP Detected %02d cores.\nGMT: %s \n", omp_get_num_procs(), ctime(&curtime));
  printf("Surfers: %d\t Waves: %d \n", NUM_SURFERS,NUM_WAVES);
  printf("AFTER_CHECK_WAIT: %d\t AFTER_SURF_WAIT: %d\n\n",AFTER_CHECK_WAIT,AFTER_SURF_WAIT);
  
  //Initialize fin locker lock

  omp_init_lock(&fin_locker_lock);

  // Initialize the fin locker
     fin_locker_count = 5;

  elapsed_time = omp_get_wtime(); // set up the sundial

  //Create and start bodysurfer threads.
  #pragma omp parallel num_threads(NUM_SURFERS) 
  {
    surfer();
  }

  //Wait for bodysurfers to finish then destroy fin locker lock
  omp_destroy_lock(&fin_locker_lock);

  elapsed_time = omp_get_wtime() - elapsed_time;
  printf("Elapsed time: %f\n",elapsed_time);

  //End program and check out the bonfire
  return 0;
}
