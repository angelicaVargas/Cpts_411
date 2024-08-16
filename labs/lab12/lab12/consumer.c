#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Queue Queue;
typedef struct Node Node;
void SendMsg(Queue** queue);
void TryReceive(Queue** queue);
int Done(Queue** queue,int done_sending);
Queue** CreateQueue(int num_threads);
void Enqueue(Queue* queue,int sender, int mesg);
void Dequeue(Queue* queue,int *src, int *mesg);

int main() {
    int num_threads = 4;
    int sendMax = 3;
    int done_sending = 0;
    
    Queue** queue = CreateQueue(num_threads);
    
    #pragma omp parallel num_threads(num_threads)
    {
        srand((unsigned) time(0));
        for (int i = 0; i < sendMax; i++) {
            SendMsg(queue);
            TryReceive(queue);          
        }
        #pragma omp atomic
        done_sending++;

        while(!Done(queue,done_sending)){
            TryReceive(queue);
        }
    }

    return 0;
}

struct Node {
    int data;
    struct Node* next;
    int sender;
};

struct Queue {
    Node* front;
    Node* rear;
    int size;
    omp_lock_t lock;
};
void SendMsg(Queue** queue){
    int myRank = omp_get_thread_num();
    int num_threads = omp_get_num_threads();
    int mesg = rand()%10000;
    int dest;
    Queue* q_p;
    do {
        dest=rand() % num_threads;  
    } while (dest== myRank);
    #pragma omp critical
        q_p = queue[dest];
        omp_set_lock(&q_p->lock);
        Enqueue(q_p, myRank, mesg);
        omp_unset_lock(&q_p->lock);
    printf("Id %d send to %d message %d\n",myRank, dest, mesg );
}

void TryReceive(Queue** queue){
    int id = omp_get_thread_num();
    int src = 0, mesg = 0;
    Queue* q_p;
    q_p = queue[id];

    if(q_p->size == 0) return;
    else if(q_p->size==1){
        #pragma omp critical
        omp_set_lock(&q_p->lock);
        Dequeue(q_p,&src,&mesg); 
        omp_unset_lock(&q_p->lock);
    }else{
        omp_set_lock(&q_p->lock);
        Dequeue(q_p,&src,&mesg);
        omp_unset_lock(&q_p->lock);
    }

    printf("*****Id %d receive from %d message %d\n",id,src,mesg);
}

void Enqueue(Queue* queue,int sender, int mesg) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = mesg;
    newNode->sender=sender;
    newNode->next = NULL;

    if (queue->rear == NULL) {
        queue->front = queue->rear = newNode;
        queue->size++;
        return;
    }

    queue->rear->next = newNode;
    queue->rear = newNode;
    queue->size++;
}

void Dequeue(Queue* queue,int *src, int *mesg) {
    if (queue->front == NULL) {
        printf("Queue is empty\n");
        return; // Return an error value
    }

    Node* temp = queue->front;
    *mesg = temp->data;
    *src =temp->sender;
    queue->front = temp->next;
    queue->size--;

    if (queue->front == NULL) {
        queue->rear = NULL;
    }
    free(temp);
}

int Done(Queue** queue,int done_sending){
    int id = omp_get_thread_num();
    int num_threads=omp_get_num_threads();
    if(queue[id]->size==0 &&  done_sending==num_threads) 
        return 1;
    else 
        return 0;
}

Queue** CreateQueue(int num_threads) {
    Queue**  queue = (Queue**)malloc(sizeof(Queue*)*num_threads);
    for(int i=0;i<num_threads;i++){
        queue[i] = (Queue*)malloc(sizeof(Queue));
        queue[i]->size=0;
        queue[i]->front = queue[i]->rear = NULL;
    }
    
    return queue;
}

int isEmpty(Queue* queue) {
    return (queue->front == NULL);
}



