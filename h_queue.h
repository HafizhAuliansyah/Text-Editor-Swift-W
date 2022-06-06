/* File        : SingleLinkedList.h */
/* Deskripsi   : ADT list berkait dengan representasi fisik pointer */
/*		 Representasi address dengan pointer */
/*		 infoQueue adalah integer */
/* Dibuat oleh : Muhammad Rizki Halomoan */
/* Tanggal     : 5/06/2022 */
#include <stdio.h>
#include "input.h"

#ifndef h_queue_H
#define h_queue_H

#define FRONT(Q) (Q).FRONT
#define REAR(Q) (Q).REAR
typedef teksEditor infoQueue;

typedef struct
{
	int FRONT;
	int REAR;
	infoQueue Info[5];
} Queue;

void CreateQueue(Queue *Q);

bool IsEmpty(Queue Q);

void Enqueue(Queue *Q, infoQueue X);

void Dequeue(Queue *Q);

void Display(Queue Q);

bool IsFull(Queue Q);

void Pop(Queue *Q);

void printInfo(Queue Q);

#endif
