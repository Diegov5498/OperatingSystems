/*  
    Programmer: Diego Vela
    Date: March 18, 2023
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#define SIZE ( sizeof(list)/sizeof(*list) )

//Global array
int list[] = {7, 12, 19, 3, 18, 4, 2, -5, 6, 15 , 8} ;  // array initially filled with unsorted numbers
int result[SIZE] = {0};                                 // same contents as unsorted array, but sorted

//Structures for passing data to worker threads
typedef struct 
{
    int* subArray;
    unsigned int size;
} SortingThreadParameters;

typedef struct 
{
    SortingThreadParameters left;
    SortingThreadParameters right;
} MergingThreadParameters;

//Functions for main process to thread
void *sortArr(void *thread_args);
void *mergeArr(void *thread_args);


//Sorting.c Sorts global array using pthread_create()
int main(int argc, char *argv[]) {
    //Declarations
    pthread_t leftID, rightID, mergeID;
   
    //Thread 1
    SortingThreadParameters *paramsLeft = malloc( sizeof( SortingThreadParameters ) );
    paramsLeft->subArray = list;
    paramsLeft->size = SIZE/2;
    if ((pthread_create(&leftID, NULL, sortArr, (void*)paramsLeft)) != 0 ) {
        return 1;
    }

    //Thread 2
    SortingThreadParameters *paramsRight = malloc( sizeof( SortingThreadParameters ) );
    paramsRight->subArray = list + paramsLeft->size;
    paramsRight->size = SIZE - paramsLeft->size;
    if ((pthread_create(&rightID, NULL, sortArr, (void*)paramsRight)) != 0 ) {
        return 1;
    }

    //wait for the sorting threads to complete
    pthread_join(leftID, NULL);
    pthread_join(rightID, NULL);

    //Merge
    MergingThreadParameters * paramsMerge = malloc( sizeof( MergingThreadParameters ) );
    paramsMerge->left = *paramsLeft;
    paramsMerge->right = *paramsRight;
    if ((pthread_create(&mergeID, NULL, mergeArr, (void*)paramsMerge)) != 0 ) {
        return 1;
    }
    
    pthread_join(mergeID, NULL);

    //Print sorted threads Array
    printf("Unsorted Array after sorting threads perform an in-place sort\n");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", list[i]);
    }
    printf("\n");
    //Print Final Array
    printf("\nSorted Array\n");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", result[i]);
    }
    printf("\n");
    
    return EXIT_SUCCESS;
}

void *sortArr(void *thread_args){
    //Declare
    SortingThreadParameters *sortMe = (SortingThreadParameters*) thread_args;
    int temp;
    //Selection Sort
    for (int i = 0; i < sortMe->size; i++) {
        for (int j = i+1; j < sortMe->size; j++) {
            if (*(sortMe->subArray + i) > *(sortMe->subArray + j)) {
                temp = *(sortMe->subArray+i);
                *(sortMe->subArray + i) = *(sortMe->subArray + j);
                *(sortMe->subArray + j) = temp;
            }
        } 
    }
    pthread_exit(NULL);
}

void *mergeArr(void *thread_args) {
    MergingThreadParameters *mergeMe =  (MergingThreadParameters*) thread_args;
    int i, j, r = 0;
    while (i < mergeMe->left.size && j < mergeMe->right.size)
    {
        if (mergeMe->left.subArray[i] < mergeMe->right.subArray[j])
        {
            result[r] = mergeMe->left.subArray[i];
            i++;
        }
        else
        {
            result[r] = mergeMe->right.subArray[j];
            j++;
        }
        r++;
    }
    
    if (i < mergeMe->left.size)
    {
        while (i < mergeMe->left.size)
        {
            result[r] = mergeMe->left.subArray[i];
            i++;
            r++;
        }
    }
    else
    {
        while (j < mergeMe->right.size)
        {
            result[r] = mergeMe->right.subArray[j];
            j++;
            r++;
        }
    }

    pthread_exit(NULL);
};