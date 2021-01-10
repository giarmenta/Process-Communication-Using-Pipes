/* By Gerardo Armenta
 * Last updated 01/10/2021
 * Times communication between parent process and child process using pipes for
 * writing and reading. 
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <sys/wait.h>

int main( ) {
	// clocks used to measure time for processes
	clock_t clock_p1;
	clock_t clock_p2;
	clock_t clock_c1;
	clock_t clock_c2;
	float timer_clock;  // used for final time 

	// pipes A and B used for connecting both processes
   	int pipe_A[2];
   	int pipe_B[2];
   	pipe(pipe_A);
  	pipe(pipe_B);

   	int p = fork();  // used to create 2 processes.

	// If p is a positive integer greater than 0 then it is the parent process
	// If p is 0 it is the child process
   	printf("This is p: %d\n", p);
 
	// Parent process
   	if (p>0) {
		char my_str[10];
		close(pipe_A[0]);	// closes read from pipe A
		clock_p1 = clock();	// saves time at this moment
	
		write(pipe_A[1], my_str, 10);	// writes through this pipe 
		close(pipe_A[1]);	// closes write from pipe A
		wait(NULL);	// waits and child starts
	
		close(pipe_B[0]);	// closes read from pipe B
		read(pipe_B[0], my_str, 10);	// reads from pipe B
		clock_p2 = clock();	// saves time at this moment
		// the difference between c2 and c1 is the time it took for the parent process to
        // finish.
		timer_clock = (float)((clock_p2-clock_p1));
		printf("The Parent process took: %f\n", timer_clock);
		close(pipe_B[0]);	// closes read pipe
   	}

	// Child process
   	else{
		char my_str[10];
		close(pipe_A[1]);	// closes write
		clock_c1 = clock();	// saves time at this moment

		read(pipe_A[0], my_str, 10);	// reads from pipe, from parent process
		close(pipe_A[0]);	// closes read
		close(pipe_B[0]);	// closes read 

		clock_c2 = clock();	// saves time at this moment
		write(pipe_B[1], my_str, 10);		// writes through this pipe
		// the difference between c2 and c1 is the time it took for the process to read
		timer_clock = (float)((clock_c2-clock_c1));	
		printf("The Child precess took: %f\n", timer_clock);
		close(pipe_B[1]);	// closes write pipe
	
		exit(0);	// process ended
	}
}
