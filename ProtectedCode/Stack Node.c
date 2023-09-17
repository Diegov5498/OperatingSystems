/*
* Stack containing race conditions
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define OPERATIONS 500

// Linked list node
typedef int value_t;
typedef struct Node
{
    value_t data;
    struct Node *next;
} StackNode;

// Stack function declarations
void    push    (value_t v, StackNode **top);
value_t pop     (           StackNode **top);
int     is_empty(           StackNode *top);

void *testStack(void*);
pthread_mutex_t mutex; // Declare a mutex object

int main(void)
{
    StackNode *top = NULL;
    pthread_t loopThreads[500];

    /*Create a loop that creates 200 threads*/
    for(int i = 0; i < 500; i++) {
        pthread_create(&loopThreads[i], NULL, testStack, (void*)top);
    }
    /*Join*/
    for(int i = 0; i < 500; i++) {
        pthread_join(loopThreads[i], NULL);
    }

    if (is_empty(top)== 1) {
        printf("Success");
    } else {
        printf("Failure");
    }

    return 0;
}

// Stack function definitions
void push(value_t v, StackNode **top)
{
    StackNode * new_node = malloc(sizeof(StackNode));

    new_node->data = v;
    new_node->next = *top;
    *top           = new_node;
}

value_t pop(StackNode **top)
{
    if (is_empty(*top)) return (value_t)0;

    value_t     data  = (*top)->data;
    StackNode * temp  = *top;
    *top              = (*top)->next;

    free(temp);

    return data;
}

int is_empty(StackNode *top) {
    if (top == NULL) return 1;
    else             return 0;
}

/*Create Test Stack function here*/
void *testStack(void* top) {
    StackNode* head = (StackNode*)top;
    //Lock the mutex before accessing the shared resource
    pthread_mutex_lock(&mutex); 
    //Code
    push(3, &head);
    push(2,&head);
    pop (&head);
    pop (&head);
    push(1,&head);
    pop (&head);
    num++;
    printf("Enter %d \n", num);
    // Unlock the mutex after accessing the shared resource
    pthread_mutex_unlock(&mutex); 
}
