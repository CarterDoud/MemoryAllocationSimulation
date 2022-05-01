/*
Carter Doud
Cis3110
Assignment 3
0931608
Simulates memory allocation
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 struct process
{	
	char id;
	int start;
	int size;
	int end;
	int swapNum;
	int priority;
	int age;
	int swapCount;
	struct process *link;
	struct process *prev;
}*front = NULL, *referenceFront = NULL, *memory = NULL;
double memUsage = 0;
double avgMem;
double avgcMem=0;
double sumAvgcMem=0;
int numProc= 0;
int numHole = 1;
double totalHoles = 0;
void insert(struct process *processtoadd,int item_priority, int swapNum);
int isEmpty();
void copy(struct process *list1, struct process **list2);
void display(struct process *print);
int del();
void deleteList(struct process* head_ref);
void removeOldestProcess(struct process* head);
int main(int argc, char *argv[])

{
	FILE* fileIn = NULL;
	char dataString [155];
	if (argc < 2)
    {
        //exits program if incorrect number of command line args are entered
        exit(0);
    }
	fileIn = fopen(argv[1], "r+");
    if (fileIn == NULL)
    {
        printf("File Empty\n");
        //checks if file is empty        
    }
	//parses file and creates queue
	while (fgets(dataString, 155, fileIn) != NULL)
	{
	    char* word;
      	char tempId;
		struct process *temp = malloc(sizeof(struct process));
		int tempSize;
		word = strtok(dataString, " ");
		sscanf(word,"%c", &tempId);
		word = strtok(NULL, " ");
		sscanf(word,"%d", &tempSize);
		temp->id=tempId;
		temp->size=tempSize;
		insert(temp,0,0);
		
	}
	copy(front,&referenceFront);
	//initializes memory to size 128
	struct process *memPtr;
	memPtr=(struct process *)malloc(sizeof(struct process));
	memPtr->size = 128;
	memPtr->start= 0;
	memPtr->end = 128;
	memPtr->id = '!';
	memPtr->link = NULL;
	memPtr->prev= NULL;
	memory = memPtr;
	int clock = 0;
	//First fit
	while(front!=NULL)
	{
		int holeFound = 0;
		struct process *tmp;
		struct process *ptr;
		ptr = memory;
		//printf("attempting to insert %c\n", front->id);
		while (ptr!=NULL)
		{
			tmp=(struct process *)malloc(sizeof(struct process));
			//checks if there is a hole in memory
			if(ptr->id == '!')
			{
				if(isEmpty())
				{
					break;
				}
				//checks if process will fit
				if (ptr->size>=front->size)
				{
					holeFound = 1;
					int holeSize = 0;
					int procSize = 0;
					holeSize = ptr->size;
					procSize = front->size;
					//changes hole to process
					ptr->id=front->id;
					ptr->size=front->size;
					ptr->age = clock;
					ptr->swapCount= front->swapCount;
					ptr->end = (ptr->start)+(front->size);					
					clock++;
					//creates new hole for remaining space
					tmp->prev = ptr;
					tmp->link = ptr-> link;
					if(ptr->link!=NULL)
					{
						ptr->link->prev=tmp;
					}
					tmp->start = ptr->end;
					tmp->id = '!';
					tmp->size = holeSize - procSize;
					tmp->end= tmp->start + tmp->size;
					ptr->link = tmp; 
					//removes process from queue
					del();
					numProc++;
					if(procSize == holeSize)
					{
						numHole--;
					}
					//calculates statistics
					memUsage=memUsage+procSize;
					avgMem=(memUsage/128)*100;
					sumAvgcMem = sumAvgcMem+avgMem;
					avgcMem= sumAvgcMem/clock;
					totalHoles= totalHoles+numHole;
					printf("%c loaded, #Processes = %d #Holes = %d memUsage = %.1f cumulative memUsage = %.1f\n",ptr->id, numProc, numHole,avgMem,avgcMem);
				}	
			}		
			ptr=ptr->link;
		}
		//if no hole found, remove oldest process
		if(holeFound==0)
		{
			removeOldestProcess(memory);
		}
	}
	printf("First Fit results\n");
	printf("Total loads =  %d, #Processes = %d, average # Holes = %.1f cumulative memUsage = %.1f\n",clock, numProc, totalHoles/clock,avgcMem);
	// end of first fit
	
	//resets variables
	copy(referenceFront,&front);
	memUsage = 0;
	avgMem;
	avgcMem=0;
	sumAvgcMem=0;
	numProc= 0;
	numHole = 1;
	totalHoles = 0;
	deleteList(memory);
	free(memPtr);
	memory = NULL;
	memPtr=(struct process *)malloc(sizeof(struct process));
	memPtr->size = 128;
	memPtr->start= 0;
	memPtr->end = 128;
	memPtr->id = '!';
	memPtr->link = NULL;
	memPtr->prev= NULL;
	memory = memPtr;
	clock = 0;
	//best fit
	while(front!=NULL)
	{
		int holeFound = 0;
		int bestSize = 0;
		struct process *tmp;
		struct process *ptr;
		struct process *holeLocation = NULL;
		ptr = memory;
		//printf("attempting to insert %c\n", front->id);
		tmp=(struct process *)malloc(sizeof(struct process));
		while (ptr!=NULL)
		{
			
			if(ptr->id == '!')
			{
				if(isEmpty())
				{
					break;
				}
				if(ptr->size>=front->size)
				{
					holeFound = 1;
					//if there are no previous holes found, set current hole to holeLocation
					if(holeLocation == NULL)
					{
						bestSize=ptr->size;
						holeLocation = ptr;
					}
					else
					{
						//if the current hole is smaller but still fits the process
						if(ptr->size<bestSize)
						{
							bestSize=ptr->size;
							holeLocation = ptr;
						}
					}
				}	
			}		
			ptr=ptr->link;
		}
		if (holeFound==1)
		{
			int holeSize = 0;
			int procSize = 0;
			//modifies hole to contain process info
			holeSize = holeLocation->size;
			procSize = front->size;
			holeLocation->id=front->id;
			holeLocation->size=front->size;
			holeLocation->age = clock;
			holeLocation->swapCount= front->swapCount;
			holeLocation->end = (holeLocation->start)+(front->size);	
			clock++;
			//creates new hole after process
			tmp->prev = holeLocation;
			tmp->link = holeLocation-> link;
			if(holeLocation->link!=NULL)
			{
				holeLocation->link->prev=tmp;
			}
			tmp->start = holeLocation->end;
			tmp->id = '!';
			tmp->size = holeSize - procSize;
			tmp->end= tmp->start + tmp->size;
			holeLocation->link = tmp; 
			del();
			numProc++;
			if(procSize == holeSize)
			{
				numHole--;
			}
			//calculates statistics
			memUsage=memUsage+procSize;
			avgMem=(memUsage/128)*100;
			sumAvgcMem = sumAvgcMem+avgMem;
			avgcMem= sumAvgcMem/clock;
			totalHoles= totalHoles+numHole;
			printf("%c loaded, #Processes = %d #Holes = %d memUsage = %.1f cumulative memUsage = %.1f\n",holeLocation->id, numProc, numHole,avgMem,avgcMem);
		}
		//if there is no suitable hole, remove the oldest process
		else if(holeFound==0)
		{
			removeOldestProcess(memory);		
		}
	}
	printf("Best Fit results\n");
	printf("Total loads =  %d, #Processes = %d, average # Holes = %.1f cumulative memUsage = %.1f\n",clock, numProc, totalHoles/clock,avgcMem);
	//resets variables
	copy(referenceFront,&front);
	memUsage = 0;
	avgMem;
	avgcMem=0;
	sumAvgcMem=0;
	numProc= 0;
	numHole = 1;
	totalHoles = 0;
	deleteList(memory);
	free(memPtr);
	memory = NULL;
	memPtr=(struct process *)malloc(sizeof(struct process));
	memPtr->size = 128;
	memPtr->start= 0;
	memPtr->end = 128;
	memPtr->id = '!';
	memPtr->link = NULL;
	memPtr->prev= NULL;
	memory = memPtr;
	clock = 0;
	//worst fit
	while(front!=NULL)
	{
		int holeFound = 0;
		int worstSize = 0;
		struct process *tmp;
		struct process *ptr;
		struct process *holeLocation = NULL;
		ptr = memory;
		//printf("attempting to insert %c\n", front->id);
		tmp=(struct process *)malloc(sizeof(struct process));
		while (ptr!=NULL)
		{
			
			if(ptr->id == '!')
			{
				if(isEmpty())
				{
					break;
				}
				if(ptr->size>=front->size)
				{
					holeFound = 1;
					//if there is no previous hole, set holeLocation
					if(holeLocation == NULL)
					{
						worstSize=ptr->size;
						holeLocation = ptr;
					}
					else
					{
						//if current hole is larger than previous worstSize and fits the process
						if(ptr->size>worstSize)
						{
							worstSize=ptr->size;
							holeLocation = ptr;
						}
					}
				}	
			}		
			ptr=ptr->link;
		}
		if (holeFound==1)
		{
			int holeSize = 0;
			int procSize = 0;
			//modifies hole to contain process info
			holeSize = holeLocation->size;
			procSize = front->size;
			holeLocation->id=front->id;
			holeLocation->size=front->size;
			holeLocation->age = clock;
			holeLocation->swapCount= front->swapCount;
			holeLocation->end = (holeLocation->start)+(front->size);	
			clock++;
			tmp->prev = holeLocation;
			tmp->link = holeLocation-> link;
			if(holeLocation->link!=NULL)
			{
				holeLocation->link->prev=tmp;
			}
			//creates new hole after process
			tmp->start = holeLocation->end;
			tmp->id = '!';
			tmp->size = holeSize - procSize;
			tmp->end= tmp->start + tmp->size;
			holeLocation->link = tmp; 
			del();
			numProc++;
			if(procSize == holeSize)
			{
				numHole--;
			}
			//calculates statistics
			memUsage=memUsage+procSize;
			avgMem=(memUsage/128)*100;
			sumAvgcMem = sumAvgcMem+avgMem;
			avgcMem= sumAvgcMem/clock;
			totalHoles= totalHoles+numHole;
			printf("%c loaded, #Processes = %d #Holes = %d memUsage = %.1f cumulative memUsage = %.1f\n",holeLocation->id, numProc, numHole,avgMem,avgcMem);
		}
		//if no hole is found, remove oldest process
		else if(holeFound==0)
		{
			removeOldestProcess(memory);	
		}
	}
	printf("Worst Fit results\n");
	printf("Total loads =  %d, #Processes = %d, average # Holes = %.1f cumulative memUsage = %.1f\n",clock, numProc, totalHoles/clock,avgcMem);
	//resets variables
	copy(referenceFront,&front);
	memUsage = 0;
	avgMem;
	avgcMem=0;
	sumAvgcMem=0;
	numProc= 0;
	numHole = 1;
	totalHoles = 0;
	deleteList(memory);
	free(memPtr);
	memory = NULL;
	memPtr=(struct process *)malloc(sizeof(struct process));
	memPtr->size = 128;
	memPtr->start= 0;
	memPtr->end = 128;
	memPtr->id = '!';
	memPtr->link = NULL;
	memPtr->prev= NULL;
	memory = memPtr;
	struct process *last = NULL;
	clock = 0;
	//next fit
	while(front!=NULL)
	{
		int holeFound = 0;
		int worstSize = 0;
		struct process *tmp;
		struct process *ptr;
		struct process *holeLocation = NULL;
		ptr = memory;
		//printf("attempting to insert %c\n", front->id);
		tmp=(struct process *)malloc(sizeof(struct process));
		while(front!=NULL)
		{
			int holeFound = 0;
			int circle = 0;
			struct process *tmp;
			struct process *ptr;
			//if this is the first process to be inserted
			if(last==NULL)
			{
				ptr = memory;
			}
			else
			{
				ptr = last;
			}
			//printf("attempting to insert %c\n", front->id);
			while (ptr!=NULL)
			{
				tmp=(struct process *)malloc(sizeof(struct process));
				if(ptr->id == '!')
				{
					if(isEmpty())
					{
						break;
					}
					if (ptr->size>=front->size)
					{
						holeFound = 1;
						int holeSize = 0;
						int procSize = 0;
						holeSize = ptr->size;
						procSize = front->size;
						ptr->id=front->id;
						ptr->size=front->size;
						ptr->age = clock;
						ptr->swapCount= front->swapCount;
						ptr->end = (ptr->start)+(front->size);
						last = ptr;
						clock++;
						tmp->prev = ptr;
						tmp->link = ptr-> link;
						if(ptr->link!=NULL)
						{
							ptr->link->prev=tmp;
						}
						tmp->start = ptr->end;
						tmp->id = '!';
						tmp->size = holeSize - procSize;
						tmp->end= tmp->start + tmp->size;
						ptr->link = tmp; 
						del();
						numProc++;
						if(procSize == holeSize)
						{
							numHole--;
						}
						//calculates statistics
						memUsage=memUsage+procSize;
						avgMem=(memUsage/128)*100;
						sumAvgcMem = sumAvgcMem+avgMem;
						avgcMem= sumAvgcMem/clock;
						totalHoles= totalHoles+numHole;
						printf("%c loaded, #Processes = %d #Holes = %d memUsage = %.1f cumulative memUsage = %.1f\n",ptr->id, numProc, numHole,avgMem,avgcMem);	
					}	
				}
				//if no hole is found, search from head of list to find hole, will only execute once
				if(ptr->link==NULL&&circle==0&&holeFound==0)
				{
					ptr=memory;
					circle=1;
				}
				else
				{
					ptr=ptr->link;
				}
			}
			if(holeFound==0)
			{
				removeOldestProcess(memory);
			}
		}
	}
	printf("Next Fit results\n");
	printf("Total loads =  %d, #Processes = %d, average # Holes = %.1f cumulative memUsage = %.1f\n",clock, numProc, totalHoles/clock,avgcMem);
	return 0;
}
/*
removeOldestProcess
head - head of list to search
finds the oldest process and deletes it
if the process' swapCount is less than 3, it will add it back to the process queue
*/
void removeOldestProcess(struct process* head)
{
	struct process *ptr,*ptr2;
	struct process *procDelete, *procQueue;
	procQueue=(struct process *)malloc(sizeof(struct process));
	ptr = head;
	int oldest = 1000;
	int swapCount = 0;
	int holeBreak = 0;
	int mergeBefore = 0;
	int mergeAfter = 0;
	while (ptr!= NULL)
	{
		if(ptr->id!='!')
		{
			if(ptr->age<=oldest)
			{
				procDelete = ptr;
				oldest= procDelete->age;
			}
			
		}
		ptr=ptr->link;
	}
	printf("%c delete\n",procDelete->id);
	procDelete->swapCount = procDelete->swapCount +1;
	swapCount = procDelete->swapCount;
	//if process has been swapped less than 3 times, add it back to queue
	if(procDelete->swapCount<3)
	{
		procQueue->id = procDelete->id;
		procQueue->size = procDelete->size;
		insert(procQueue,0,swapCount);
	}
	memUsage=memUsage - procDelete->size;
	//checks if there is a hole after the process, attempts to merge
	if(procDelete->link!=NULL)
	{
		ptr2 = procDelete->link;
		while(holeBreak ==0)
		{
			if(ptr2!= NULL)
			{
				
				if(ptr2->id == '!')
				{
					if(ptr2->size!=0)
					{
						mergeAfter=1;
						procDelete->end=ptr2->end;
						procDelete->size = procDelete->size+ptr2->size;
						procDelete->link = ptr2->link;
						
						if(ptr2->link !=NULL)
						{
							ptr2->link->prev=procDelete;
						}
						else
						{

						}
						free(ptr2);
						ptr2 = procDelete->link;
					}
					else
					{
						holeBreak=1;
					}	
				}
				else
				{
					holeBreak=1;
				}
			}
			else
			{
				holeBreak=1;
			}
		}
	}
	holeBreak=0;
	//checks if there is a hole before the process, attempts to merge holes
	if (procDelete->prev!=NULL)
	{
		ptr2 = procDelete->prev;
		while(holeBreak==0)
		{
			if(ptr2!=NULL)
			{
				if(ptr2->id=='!')
				{
					if(ptr2->size!=0)
					{
						mergeBefore=1;
						procDelete->start=ptr2->start;
						procDelete->size=procDelete->size+ptr2->size;
						procDelete->prev=ptr2->prev;
						if(ptr2->prev != NULL)
						{
							ptr2->prev->link=procDelete;
						}
						else
						{
							memory=procDelete;
						}
						free(ptr2);
						ptr2=procDelete->prev;
					}
					else
					{
						holeBreak=1;
					}
				}
				else
				{
					holeBreak=1;
				}
			}
			else
			{
				holeBreak=1;
			}
		}
	}
	//calculates number of holes
	if(mergeBefore==0&&mergeAfter==0)
	{
		numHole++;
	}
	else if(mergeBefore==1&&mergeAfter==1)
	{
		numHole--;
	}
	numProc--;
	procDelete->id = '!';
	free(procQueue);
}
/*
copy
list1 - list to copy from
list2 - list to write to
referenced and modified
source:
http://www.dailyfreecode.com/code/copy-linked-list-another-recursion-2824.aspx
*/
void copy(struct process *list1, struct process **list2)
{
	if ( list1 != NULL )
    {
        *list2 = malloc ( sizeof ( struct process ) ) ;

        ( *list2 )->id = list1->id ;
		( *list2 )->size = list1->size ;
		( *list2 )->swapCount = list1->swapCount ;
        ( *list2 )-> link = NULL ;

        copy ( list1 -> link, &( ( *list2 ) -> link ) ) ;
    }
	
}
/*
insert
processtoadd - pointer to process to add to queue
priority - priority of item in queue, not used in this program
swapNum - swapNumber of process
referenced and modified: orriginal code:https://codeitaway.wordpress.com/2012/09/22/c-program-of-priority-queue-using-linked-list/
*/
void insert(struct process *processtoadd,int item_priority, int swapNum)
{
	struct process *tmp,*p;

	tmp=(struct process *)malloc(sizeof(struct process));
	if(tmp==NULL)
	{
		printf("Memory not available\n");
	return;
	}
		tmp->id=processtoadd->id;
		tmp->size = processtoadd->size;
		tmp->swapCount= swapNum;
		/*Queue is empty or item to be added has priority more than first element*/
		if( isEmpty() || item_priority < front->priority)
		{
			tmp->link=front;
			front=tmp;
		}
		else
		{
			p = front;
			while( p->link!=NULL && p->link->priority<=item_priority )
				p=p->link;
				tmp->link=p->link;
				p->link=tmp;
		}
}
/*
checks if front is null
referenced and modified: orriginal code:https://codeitaway.wordpress.com/2012/09/22/c-program-of-priority-queue-using-linked-list/
*/
int isEmpty()
{
	if( front == NULL )
	return 1;
	else
	return 0;

}
/*
display
print - pointer to head of list to print
prints contents of list
referenced and modified: orriginal code:https://codeitaway.wordpress.com/2012/09/22/c-program-of-priority-queue-using-linked-list/
*/
void display(struct process *print)
{
	struct process *ptr;
	ptr=print;
	if( isEmpty() )
		printf("Queue is empty\n");
	else
	{    printf("Queue is :\n");
		printf("Priority       Item\n");
		while(ptr!=NULL)
		{
			printf("%c       %d     %d\n",ptr->id,ptr->size,ptr->swapCount);
			ptr=ptr->link;
		}
	}
}
/*
del
deletes head of queue
referenced and modified: orriginal code:https://codeitaway.wordpress.com/2012/09/22/c-program-of-priority-queue-using-linked-list/
*/
int del()
{
	struct process *tmp;
	int item;
	if( isEmpty() )
	{
		printf("Queue Underflow\n");
		exit(1);
	}
	else
	{
		tmp=front;
		front=front->link;
		free(tmp);
	}
	return item;
}
/*
deleteList
head_ref - head of list to delete
deletes all nodes in the list
*/

void deleteList(struct process* head_ref)
{
	if(head_ref)
	{
		deleteList(head_ref->link);
		free(head_ref);
	}
}