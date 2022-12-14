#include <stdio.h>     /* standard I/O functions                         */
#include <stdlib.h>    /* exit                                           */
#include <string.h>    /* memset                                         */
#include <unistd.h>    /* standard unix functions, like getpid()         */
#include <signal.h>    /* signal name macros, and the signal() prototype */

/* GLOBALS */
/* define the Ctrl-C counter, initialize it with zero. */
int ctrl_c_count = 0;
/* could be useful for the alarm signal handler & related logic */
int got_response = 0;


#define CTRL_C_THRESHOLD 5
#define TIMEOUT_SECONDS 3

/* the Ctrl-C signal handler */
/* you may need to modify this function */
void catch_int(int sig_num)
{
  /* increase count, and check if threshold was reached */
  ctrl_c_count++;

  if (ctrl_c_count >= CTRL_C_THRESHOLD) {
    char answer[30];
    /* prompt the user to tell us if to really
     * exit or not */
    printf("\nReally exit? [Y/n]: ");
    alarm(3);
    fflush(stdout);
    fgets(answer, sizeof(answer), stdin);
    //alarm(3);
    if (answer[0] == 'n' || answer[0] == 'N') {
      alarm(0);
      printf("\nContinuing\n");
      fflush(stdout);
      /*
       * Reset Ctrl-C counter
       */
       ctrl_c_count = 0;
    }
    else {
      alarm(0);
      printf("\nExiting...\n");
      fflush(stdout);
      exit(0);
    }
  }
}

/* the Ctrl-Z signal handler */
void catch_tstp(int sig_num)
{
  /* print the current Ctrl-C counter */
  printf("\n\nSo far, '%d' Ctrl-C presses were counted\n\n", ctrl_c_count);
  fflush(stdout);
}

/* STEP - 1 (20 points) */
/* Implement the alarm signal handler */
/* If the user DOES NOT RESPOND before the alarm time elapses, the program should exit */
/* If the user RESPONDS before the alarm time elapses, the alarm should be cancelled */
/* Hint: The alarm may need to be registered elsewhere */
//YOUR CODE

void catch_alrm(int sig_num) {
  //  printf("Exiting.....too slow");
  // fflush(stdout);
  printf("\n\nUser taking too long to respond. Exiting . . .\n\n");
  exit(0);
  // if(time > 3) {
  //   printf("User taking too long to respsond. Exiting . . .\n\n");
  //   fflush(stdout);
  //   exit(0);
  // }MyLinkedList_Template
  // else {
  //   printf("Continuing\n");
  //   fflush(stdout);
  // }

}

int main(int argc, char* argv[])
{
  /* create sigaction structs for each signal we want to handle */
  /* this struct type stores information needed to override/specify a signal handler, along with related options */
  /* be sure to familiarize with the fields of this struct type as you will need to set some of them */
  struct sigaction sa_int, sa_tstp, sa_alrm;
  // struct sigaction ctrl_c[SIGINT];
  // sigaction (SIGINT, *catch_int, NULL);
  // sigaction (SIGTSTP, *catch_tstp, NULL);
  // struct sigaction ctrl_z[SIGTSTP];
  //signal(SIGALRM, sa_alrm);
  //alarm(4);
  // sigaction(SIGINT, catch_int, NULL);
  // sigaction(SIGTSTP, catch_tstp, NULL);



  /* STEP - 2 (10 points) */
  /* clear the memory at each sigaction struct above by filling up each of their memory ranges with all 0 bytes */
  /* this acts to clear any fields in the initialized structs that may have garbage values */
  /* hint: use the function memset - type "man memset" on the terminal and take reference from it */
  //YOUR CODE
  memset(&sa_int, 0, sizeof sa_int);
  memset(&sa_tstp, 0, sizeof sa_tstp);
  memset(&sa_alrm, 0, sizeof sa_alrm);

  /* used to set a signal masking set. */
  /* this should be used somewhere within the sigaction structs to specify which signals to block/not block within the signal handler functions */
  sigset_t mask_set;


  /* STEP - 3 (10 points) */
  /* setup mask_set - fill up the mask_set with all the signals to block within signal handler functions */
  //YOUR CODE
  sigfillset(&mask_set);


  /* STEP - 4 (10 points) */
  /* ensure in the mask_set that the alarm signal does not get blocked while in another signal handler */
  /* this is because we want to be able to raise an alarm signal from within the other signal handlers */
  //YOUR CODE
  sigdelset(&mask_set, SIGALRM);


  /* STEP - 5 (30 points) */
  /* set signal handlers for SIGINT, SIGTSTP and SIGALRM */
  /* keep in mind which fields of the sigaction structs to fill before "registering" the sigactions */
  //YOUR CODE
  sa_int.sa_handler = catch_int;
  sa_int.sa_mask = mask_set;
  sigaction(SIGINT, &sa_int, NULL);
  sa_tstp.sa_handler = catch_tstp;
  sa_tstp.sa_mask = mask_set;
  sigaction(SIGTSTP, &sa_tstp, NULL);
  sa_alrm.sa_handler = catch_alrm;
  sa_alrm.sa_mask = mask_set;
  sigaction(SIGALRM, &sa_alrm, NULL);


  /* STEP - 6 (10 points) */
  /* ensure that the program keeps running to receive the signals */
  //YOUR CODE
  for(int i=1;;i++){    //Infinite loop
    pause();
  }


  return 0;
}
