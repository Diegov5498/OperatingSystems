#include <sys/types.h>
#include <sys/time.h>
#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]) { // TODO: FIX SHARED MEMORY
    //Declaration
    struct timeval startTime, endTime, elapsedTime;
    /* the size (in bytes) of shared memory object */
    const int SIZE = 4096;
    /* name of the shared memory object */
    const char *name = "Shared memory";
    /* pointer to shared memory obect */
    struct timeval *shdMemory = (struct timeval *)malloc(sizeof(struct timeval));
    /* shared memory file descriptor */
    int fd;
    /* create the shared memory object */
    fd = shm_open(name,O_CREAT | O_RDWR,0666);
    /* configure the size of the shared memory object */ 
    ftruncate(fd, SIZE);
    /* memory map the shared memory object */
    shdMemory = (struct timeval *)mmap(0, sizeof(struct timeval), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    
    close(fd);

    pid_t pid = fork();
    if (pid < 0) { fprintf(stderr, "Fork Failed"); }    

    if (pid == 0) {
        gettimeofday(&startTime, 0);
        //Write to shared memory segment
        //shdMemory = &startTime;                                 
        
        //FIX
        memcpy(shdMemory, &startTime, sizeof(struct timeval));

        execvp(argv[1], argv+1);

        exit(0);
    } 
    else {
        //For child to finish and get end time
        wait(NULL);
        gettimeofday(&endTime, 0);
        //Read starting time from shared memory                               
        //FIX
        memcpy(shdMemory, &startTime, sizeof(struct timeval));
        //Calculate
        timersub(&endTime, &startTime, &elapsedTime);
        //Print Elapsed time
        printf("\nElapsed time: %ld.%06d seconds\n", elapsedTime.tv_sec, elapsedTime.tv_usec);
    }

    //EXIT SUCCESS
    return 0;
}