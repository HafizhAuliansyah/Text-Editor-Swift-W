#include <stdio.h>
#include <stdlib.h>
#include "h_queue.h"

void CreateQueue(Queue *Q)
{
	FRONT(*Q) = -1;
	REAR(*Q) = -1;
}

bool IsEmpty(Queue Q)
{
	return (FRONT(Q) == -1 and REAR(Q) == -1);
}

bool IsFull(Queue Q)
{
	return (REAR(Q) == 4 or REAR(Q) == FRONT(Q) - 1);
}

void Enqueue(Queue *Q, infoQueue X)
{
	if (!IsFull(*Q))
	{
		if (IsEmpty(*Q))
		{
			FRONT(*Q) = 0;
			REAR(*Q) = 0;
		}
		else
		{
			if (REAR(*Q) == 4)
			{
				REAR(*Q) = 0;
			}
			else
			{
				REAR(*Q) += 1;
			}
		}
	}
	else
	{
		Dequeue(Q);
		if (REAR(*Q) == 4)
		{
			REAR(*Q) = 0;
		}
		else
		{
			REAR(*Q) += 1;
		}
	}
	Q->Info[REAR(*Q)] = X;
}

void Dequeue(Queue *Q)
{
	if (FRONT(*Q) == REAR(*Q))
	{
		CreateQueue(Q);
	}
	if (!IsEmpty(*Q))
	{
		if (FRONT(*Q) == 4)
		{
			FRONT(*Q) = 0;
		}
		else
		{
			FRONT(*Q) += 1;
		}
	}
}

void Display(Queue Q)
{
	printf("\nFRONT : %d\n", FRONT(Q));
	printf("REAR : %d\n", REAR(Q));
}

void Pop(Queue *Q)
{
	if (FRONT(*Q) == REAR(*Q))
	{
		CreateQueue(Q);
	}
	if (!IsEmpty(*Q))
	{
		if (REAR(*Q) == 0)
		{
			REAR(*Q) = 4;
		}
		else
		{
			REAR(*Q) -= 1;
		}
	}
}

void printInfo(Queue Q)
{
	for (int i = 0; i < 5; i++)
	{
		printf("%d ", Q.Info[i]);
	}
}
