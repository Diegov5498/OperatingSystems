#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>

int main(int argc, char *argv[]) { //COMPLETE PROBABLY
    //Declaration
    struct timeval startTime, endTime, elapsedTime;
    //myPipe[0] - Read  |  myPipe[1] - Write
    int myPipe[2];  
    pipe(myPipe);
    int id = fork();
    
    //Child Process
    if (id == 0) {
        //Read Pipe
        close(myPipe[0]);
        //Get startTime
        gettimeofday(&startTime, NULL);
        //Write Pipe
        if (write(myPipe[1], &startTime, sizeof(struct timeval)) == -1) {
            printf("Error detected in Pipe Child\n");
            return 2;
        }
        close(myPipe[1]);
        //Execute Command
        execvp(argv[1], argv+1);
    //Parent Process
    } else {
        //Wait for Child Process
        wait();
        //Get endTime
        gettimeofday(&endTime, NULL);
        //Pipe
        close(myPipe[1]);
        if (read(myPipe[0], &startTime, sizeof(struct timeval)) == -1) {
            printf("Error detected in Pipe Parent\n");
            return 1;
        }
        close(myPipe[0]);
        //Calculate
        timersub( &endTime, &startTime, &elapsedTime );
        //Print Elapsed time
        printf( "\nElapsed time: %d.%06d seconds\n", elapsedTime.tv_sec, elapsedTime.tv_usec );
    }

    //EXIT SUCCESS
    return 0;
}