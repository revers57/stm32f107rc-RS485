#include "stm32f10x.h"
#include "stdio.h"
#include <stdlib.h>

#define BUFFER_SIZE 256

typedef struct {
	uint8_t buff[BUFFER_SIZE];
	uint8_t front;
	uint8_t rear;
	uint8_t count;
} cbuff_;

void cbuff_init(cbuff_ * q)
{
    q->front =  0;
    q->rear  = -1;
    q->count =  0;
}
/*Check Queue is full or not*/
uint8_t isFull(cbuff_ * q)
{
    int full=0;
     
    if(q->count == BUFFER_SIZE)
        full = 1;   
 
    return full;
}
 
/*Check Queue is empty or not*/
uint8_t isEmpty(cbuff_ * q)
{
    int empty=0;
     
    if(q->count == 0)
        empty = 1;  
 
    return empty;
}
 
/*To insert item into circular queue.*/
void insert(cbuff_ * q, int item)
{
    if( isFull(q) )
    {
        printf("\nQueue Overflow");
        return;
    }
     
    q->rear = (q->rear+1)%BUFFER_SIZE;
    q->buff[q->rear] = item;
     
    q->count++;
  
    printf("\nInserted item : %d",item);
}
 
/*To delete item from queue.*/
int deleteCirQueue(cbuff_ * q, int *item)
{
    if( isEmpty(q) )
    {
        printf("\nQueue Underflow");
        return -1;
    }
 
    *item    = q->buff[q->front];
 
    q->front = (q->front+1)%BUFFER_SIZE;
     
    q->count--;
 
    return 0;
}










